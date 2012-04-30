static void
foo(void)
{
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);

	pthread_rwlock_init(&rwlock);
	pthread_rwlock_rdlock(&rwlock);
	pthread_rwlock_wrlock(&rwlock);
	pthread_rwlock_unlock(&rwlock);
	pthread_rwlock_destroy(&rwlock);

	pthread_cond_init(&cond, NULL);
	pthread_cond_signal(&cond);
	pthread_cond_broadcast(&cond);
	pthread_cond_wait(&cond, &mutex);
	pthread_cond_destroy(&cond);

	pthread_barrier_init(&barrier, NULL, 42);
	pthread_barrier_wait(&barrier);
	pthread_barrier_destroy(&barrier);
}
