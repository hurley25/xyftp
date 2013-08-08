/*
 * =====================================================================================
 *
 *       Filename:  thread_pool.h
 *
 *    Description:  线程池的实现
 *
 *        Version:  1.0
 *        Created:  2013年08月07日 16时17分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

// 任务的定义
typedef struct job
{
	void *(*job_callback)(void *); 	// 任务的回调函数
	void *arg; 			// 回调函数参数
	struct job *next; 		// 下一个任务
}job_t;

// 线程池管理结构
typedef struct thread_pool
{
	int thread_num; 		// 线程个数
	int free_thread_num; 		// 空闲线程个数
	int queue_max_num; 		// 最大任务个数
	int queue_curr_num; 		// 当前任务个数
	int resize_now; 		// 是否刚刚调整过尺寸
	job_t *head; 			// 队列头指针
	job_t *tail; 			// 队列尾指针
	pthread_t *threads; 		// 所有线程的 pthread_t
	pthread_mutex_t mutex; 		// 互斥信号量
	pthread_cond_t queue_empty; 	// 队列为空的条件变量
	pthread_cond_t queue_not_empty; // 队列非空的条件变量
	pthread_cond_t queue_not_full; 	// 队列不再满的条件变量
	int queue_close; 		// 任务队列是否关闭
	int pool_close; 		// 线程池是否关闭
}thread_pool_t;

// 初始化线程池
thread_pool_t *thread_pool_init(int thread_num, int queue_max_num);

// 调整线程池大小
int thread_pool_resize(thread_pool_t *thread_pool, int thread_num, int queue_max_num);

// 销毁线程池
int thread_pool_destroy(thread_pool_t *thread_pool);

// 向线程池添加任务
int thread_pool_add_job(thread_pool_t *thread_pool, void *(job_callback)(void *), void *arg);

// 线程的处理函数
void *thread_function(void *arg);

#endif 	// THREAD_POOL_H_
