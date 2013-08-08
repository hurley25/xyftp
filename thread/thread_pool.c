/*
 * =====================================================================================
 *
 *       Filename:  thread_pool.c
 *
 *    Description:  线程池的实现
 *
 *        Version:  1.0
 *        Created:  2013年08月07日 16时16分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

// 初始化线程池
thread_pool_t *thread_pool_init(int thread_num, int queue_max_num)
{
	thread_pool_t *thread_pool = NULL;

	do {
		thread_pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
		if (thread_pool == NULL) {
			break;
		}
		thread_pool->thread_num = thread_num;
		thread_pool->free_thread_num = thread_num;
		thread_pool->queue_max_num = queue_max_num;
		thread_pool->resize_now = 0;
		thread_pool->queue_curr_num = 0;
		thread_pool->head = thread_pool->tail = NULL;

		if (pthread_mutex_init(&(thread_pool->mutex), NULL)) {
			break;
		}
		if (pthread_cond_init(&(thread_pool->queue_empty), NULL)) {
			break;
		}
		if (pthread_cond_init(&(thread_pool->queue_not_empty), NULL)) {
			break;
		}
		if (pthread_cond_init(&(thread_pool->queue_not_full), NULL)) {
			break;
		}
		thread_pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
		if (thread_pool->threads == NULL) {
			break;
		}
		thread_pool->queue_close = 0;
		thread_pool->pool_close = 0;
		int i;
		for (i = 0; i < thread_pool->thread_num; i++) {
			if (pthread_create(&(thread_pool->threads[i]), NULL, 
							thread_function, (void *)thread_pool)) {
				free(thread_pool->threads);
				break;
			}
		}
		return thread_pool;
	} while (0);

	free(thread_pool);

	return NULL;
}

// 销毁线程池
int thread_pool_destroy(thread_pool_t *thread_pool)
{
	pthread_mutex_lock(&(thread_pool->mutex));
	// 线程池已经退出了，就直接退出
	if (thread_pool->queue_close || thread_pool->pool_close) {
		pthread_mutex_unlock(&(thread_pool->mutex));
		return -1;
	}

	// 置队列关闭标记
	thread_pool->queue_close = 1;
	
	// 等待队列为空
	while (thread_pool->queue_curr_num != 0) {
		pthread_cond_wait(&(thread_pool->queue_empty), &(thread_pool->mutex));
	}

	// 置线程池关闭标记
	thread_pool->pool_close = 1;
	pthread_mutex_unlock(&(thread_pool->mutex));

	// 唤醒线程池中正在阻塞的线程
	pthread_cond_broadcast(&(thread_pool->queue_not_empty));
	// 唤醒添加任务的threadpool_add_job函数
	pthread_cond_broadcast(&(thread_pool->queue_not_full));

	int i;
	for (i = 0; i < thread_pool->thread_num; i++) {
		pthread_join(thread_pool->threads[i], NULL);
	}
	
	// 清理资源
	pthread_mutex_destroy(&(thread_pool->mutex));
	pthread_cond_destroy(&(thread_pool->queue_empty));
	pthread_cond_destroy(&(thread_pool->queue_not_empty));
	pthread_cond_destroy(&(thread_pool->queue_not_full));
	free(thread_pool->threads);

	job_t *free_job;
	while (thread_pool->head != NULL) {
		free_job = thread_pool->head;
		thread_pool->head = thread_pool->head->next;
		free(free_job);	
	}

	free(thread_pool);

	return 0;
}

// 调整线程池大小
// @return -1 参数错误
// @return -2 申请内存错误
// @return -3 线程创建错误
int thread_pool_resize(thread_pool_t *thread_pool, int thread_num, int queue_max_num)
{
	// 不允许减小线程池规模
	if (thread_num < thread_pool->thread_num || queue_max_num < thread_pool->queue_max_num) {
		return -1;
	}
	pthread_mutex_lock(&(thread_pool->mutex));
	thread_pool->queue_max_num = queue_max_num;
	
	thread_pool->threads = (pthread_t *)realloc(thread_pool->threads, sizeof(pthread_t) * thread_num);
	if (thread_pool->threads == NULL) {
		return -2;
	}
	int i;
	for (i = thread_pool->thread_num; i < thread_num; i++) {
		if (pthread_create(&(thread_pool->threads[i]), NULL, 
						thread_function, (void *)thread_pool)) {
			return -3;
		}
	}
	thread_pool->free_thread_num += (thread_num - thread_pool->thread_num);
	thread_pool->thread_num = thread_num;
	thread_pool->resize_now = 1;
	pthread_mutex_unlock(&(thread_pool->mutex));

	return 0;
}

// 向线程池添加任务
// @ return  0 表示正常
// @ return  1 表示队列已满或所有线程忙碌
// @ return -1 表示不可抗拒错误
int thread_pool_add_job(thread_pool_t *thread_pool, void *(job_callback)(void *), void *arg)
{
	pthread_mutex_lock(&(thread_pool->mutex));
	/*
	// 队列满的时候
	while ((thread_pool->queue_curr_num == thread_pool->queue_max_num) && 
		!(thread_pool->queue_close || thread_pool->pool_close)) {
		pthread_cond_wait(&(thread_pool->queue_not_full), &(thread_pool->mutex));
	}*/

	if ((thread_pool->queue_curr_num == thread_pool->queue_max_num) && 
		!(thread_pool->queue_close || thread_pool->pool_close)) {
		// 不可以阻塞在这里，否则主线程会阻塞，队列满了立即返回
		pthread_mutex_unlock(&(thread_pool->mutex));
		return 1;
	}

	// 所有线程都在忙的时候，不再添加任务
	if (thread_pool->free_thread_num == 0) {
		pthread_mutex_unlock(&(thread_pool->mutex));
		return 1;
	}

	// 队列关闭或者线程池关闭则退出
	if (thread_pool->queue_close || thread_pool->pool_close) {
		pthread_mutex_unlock(&(thread_pool->mutex));
		return -1;
	}

	job_t *new_job = (job_t *)malloc(sizeof(job_t));
	if (new_job == NULL) {
		pthread_mutex_unlock(&(thread_pool->mutex));
		return -1;
	}
	new_job->job_callback = job_callback;
	new_job->arg = arg;
	new_job->next = NULL;
	
	thread_pool->queue_curr_num++;
	if (thread_pool->head == NULL) {
		thread_pool->head = thread_pool->tail = new_job;
		// 告诉线程池中休眠的线程们有活干了
		pthread_cond_broadcast(&(thread_pool->queue_not_empty));
	} else {
		thread_pool->tail->next = new_job;
		thread_pool->tail = new_job;
	}

	// 刚刚调整过线程池尺寸的话，要通知新创建后沉睡的家伙们起来干活了
	if (thread_pool->resize_now == 1) {
		thread_pool->resize_now = 0;
		pthread_cond_broadcast(&(thread_pool->queue_not_empty));
	
	}
	pthread_mutex_unlock(&(thread_pool->mutex));
	
	return 0;
}

// 线程的处理函数
void *thread_function(void *arg)
{
	thread_pool_t *thread_pool = (thread_pool_t *)arg;
	job_t *curr_job = NULL;

	while (true) {
		pthread_mutex_lock(&(thread_pool->mutex));
		while ((thread_pool->queue_curr_num == 0) && !thread_pool->pool_close) {
			// 等待队列不再是空的消息
			pthread_cond_wait(&(thread_pool->queue_not_empty), &(thread_pool->mutex));
		}
		// 线程池关闭的时候，线程退出
		if (thread_pool->pool_close) {
			pthread_mutex_unlock(&(thread_pool->mutex));
			pthread_exit(NULL);
		}

		// 拿取一个任务
		thread_pool->queue_curr_num--;
		curr_job = thread_pool->head;
		if (thread_pool->queue_curr_num == 0) {
			thread_pool->head = thread_pool->tail = NULL;
			// 发出队列为空的信号(销毁函数需要)
			pthread_cond_signal(&(thread_pool->queue_empty));
		} else {
			thread_pool->head = thread_pool->head->next;
		}
		
		// 刚刚从队满转换到非空，通知任务添加函数可以工作了
		if (thread_pool->queue_curr_num == thread_pool->queue_max_num - 1) {
			pthread_cond_broadcast(&(thread_pool->queue_not_full));
		}
		thread_pool->free_thread_num--;
		pthread_mutex_unlock(&(thread_pool->mutex));

#ifdef FTP_DEBUG
		char info[100];
		sprintf(info, "Thread %lu get a job", pthread_self());
		xyftp_print_info(LOG_INFO, info);
#endif
		// 执行任务里面的函数
		curr_job->job_callback(curr_job->arg);
		free(curr_job);
		curr_job = NULL;

		pthread_mutex_lock(&(thread_pool->mutex));
		thread_pool->free_thread_num++;
		pthread_mutex_unlock(&(thread_pool->mutex));

	}
}

#if 	0
// 线程池测试代码
void *work(void *arg)
{
	char *p = (char *)arg;
	printf("thread_pool callback fuction : %s.\n", p);
	sleep(1);
}

int main(int argc, int argv)
{
	char *args[] = {
		"1",  "2",  "3",  "4",  "5",
		"6",  "7",  "8",  "9",  "10",
		"11", "12", "13", "14", "15",
		"16", "17", "18", "19", "20",
		"21", "22", "23", "24", "25",
		"26", "27", "28", "29", "30"};

	thread_pool_t *thread_pool = thread_pool_init(1, 2);

	int i = 0, tag, value;
	for (i = 0; i < 30; i++) {
		do {
			tag = thread_pool_add_job(thread_pool, work, args[i]);
			if (tag == 1) {
				value = thread_pool_resize(thread_pool, 
					thread_pool->thread_num * 2, thread_pool->queue_max_num * 2);
				if (value == -1) {
					printf("参数错误!\n");
					exit(-1);
				} else if (value == -2) {
					printf("申请内存错误!\n");
					exit(-1);
				} else if (value == -3) {
					printf("线程创建错误!\n");
					exit(-1);
				}
			}
		}while (tag != 0);
	}


	sleep(2);
	thread_pool_destroy(thread_pool);

	return 0;
}
#endif
