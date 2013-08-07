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
		thread_pool->queue_max_num = queue_max_num;
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

// 向线程池添加任务
int thread_pool_add_job(thread_pool_t *thread_pool, void *(job_callback)(void *), void *arg)
{
	pthread_mutex_lock(&(thread_pool->mutex));
	// 队列满的时候等待
	while ((thread_pool->queue_curr_num == thread_pool->queue_max_num) && 
		!(thread_pool->queue_close || thread_pool->pool_close)) {
		pthread_cond_wait(&(thread_pool->queue_not_full), &(thread_pool->mutex));
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
		pthread_mutex_unlock(&(thread_pool->mutex));
		
		// 执行任务里面的函数
		curr_job->job_callback(curr_job->arg);
		free(curr_job);
		curr_job = NULL;
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
	thread_pool_t *thread_pool = thread_pool_init(5, 10);

	thread_pool_add_job(thread_pool, work, "1");
	thread_pool_add_job(thread_pool, work, "2");
	thread_pool_add_job(thread_pool, work, "3");
	thread_pool_add_job(thread_pool, work, "4");
	thread_pool_add_job(thread_pool, work, "5");
	thread_pool_add_job(thread_pool, work, "6");
	thread_pool_add_job(thread_pool, work, "7");
	thread_pool_add_job(thread_pool, work, "8");
	thread_pool_add_job(thread_pool, work, "9");
	thread_pool_add_job(thread_pool, work, "10");
	thread_pool_add_job(thread_pool, work, "11");
	thread_pool_add_job(thread_pool, work, "12");
	thread_pool_add_job(thread_pool, work, "13");
	thread_pool_add_job(thread_pool, work, "14");
	thread_pool_add_job(thread_pool, work, "15");
	thread_pool_add_job(thread_pool, work, "16");
	thread_pool_add_job(thread_pool, work, "17");
	thread_pool_add_job(thread_pool, work, "18");
	thread_pool_add_job(thread_pool, work, "19");
	thread_pool_add_job(thread_pool, work, "20");
	thread_pool_add_job(thread_pool, work, "21");
	thread_pool_add_job(thread_pool, work, "22");
	thread_pool_add_job(thread_pool, work, "23");
	thread_pool_add_job(thread_pool, work, "24");
	thread_pool_add_job(thread_pool, work, "25");
	thread_pool_add_job(thread_pool, work, "26");
	thread_pool_add_job(thread_pool, work, "27");
	thread_pool_add_job(thread_pool, work, "28");
	thread_pool_add_job(thread_pool, work, "29");
	thread_pool_add_job(thread_pool, work, "30");

	sleep(2);
	thread_pool_destroy(thread_pool);

	return 0;
}
#endif
