#ifndef __THREADS_h__
#define __THREADS_h__

#include <pthread.h>
#include <assert.h>
#include <sched.h>


                                                                                
void
Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
	       void *(*start_routine)(void*), void *arg)
{
    int count = pthread_create(thread, attr, start_routine, arg);
    assert(count == 0);
}

void
Pthread_join(pthread_t thread, void **value_ptr)
{
    int count = pthread_join(thread, value_ptr);
    assert(count == 0);
}
void
Pthread_mutex_lock(pthread_mutex_t *m)
{
    int count = pthread_mutex_lock(m);
    assert(count == 0);
}

void
Pthread_mutex_unlock(pthread_mutex_t *m)
{
    int count = pthread_mutex_unlock(m);
    assert(count == 0);
}
#endif
