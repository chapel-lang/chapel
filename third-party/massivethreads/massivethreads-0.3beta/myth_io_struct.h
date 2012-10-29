#ifndef MYTH_IO_STRUCT_H_
#define MYTH_IO_STRUCT_H_

#ifdef MYTH_WRAP_SOCKIO

#define MYTH_FD_MAP_SIZE 65536
#define MYTH_FD_MAP_KEY(fd) ((fd)%65536)
//#define MYTH_FD_MAP_KEY(fd) 0

//Map structure for searching a blocked list associated with a fd
static inline myth_fd_map_t myth_fd_map_init()
{//初期化
	myth_fd_map_t ret;
	ret=myth_malloc(sizeof(myth_fd_map));
	ret->entry=myth_malloc(sizeof(myth_fd_map_entry)*MYTH_FD_MAP_SIZE);
	int i;
	for (i=0;i<MYTH_FD_MAP_SIZE;i++){
		ret->entry[i].data=myth_malloc(sizeof(void*));
		ret->entry[i].key=myth_malloc(sizeof(int));
		ret->entry[i].size=0;
		ret->entry[i].bufsize=1;
		myth_internal_lock_init(&ret->entry[i].lock);
	}
	//ret->size=0;
	myth_internal_lock_init(&ret->lock);
	return ret;
}

static inline void myth_fd_map_set(myth_running_env_t env,myth_fd_map_t fm,int fd,myth_io_struct_perfd_t data)
{//Set fd_map to entry
	//Setting NULL as value is forbidden because NULL is returned when lookup fails
	myth_assert(data);
	int i;
	myth_fd_map_entry_t fe;
	myth_internal_lock_lock(&fm->lock);
	fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
	myth_internal_lock_lock(&fe->lock);
	for (i=0;i<fe->size;i++){
		if (fe->key[i]==fd){
			fe->data[i]=data;
			myth_internal_lock_unlock(&fe->lock);
			myth_internal_lock_unlock(&fm->lock);
			return;
		}
	}
	fe->size++;
	if (fe->size>fe->bufsize){
		//If list size is full, reallocate
		int new_bufsize=fe->bufsize*2;
		fe->key=myth_flrealloc(env->rank,sizeof(int)*fe->bufsize,fe->key,sizeof(int)*new_bufsize);
		fe->data=myth_flrealloc(env->rank,sizeof(void*)*fe->bufsize,fe->data,sizeof(void*)*new_bufsize);
		fe->bufsize=new_bufsize;
	}
	fe->key[fe->size-1]=fd;
	fe->data[fe->size-1]=data;
	myth_internal_lock_unlock(&fe->lock);
	//fm->size++;
	myth_internal_lock_unlock(&fm->lock);
}

static inline void myth_fd_map_delete(myth_fd_map_t fm,int fd)
{
	int i,j;
	myth_fd_map_entry_t fe;
	myth_internal_lock_lock(&fm->lock);
	fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
	myth_internal_lock_lock(&fe->lock);
	for (i=0;i<fe->size;i++){
		if (fe->key[i]==fd){
			for (j=i+1;j<fe->size;j++){
				fe->key[j-1]=fe->key[j];
				fe->data[j-1]=fe->data[j];
			}
			fe->size--;
			myth_internal_lock_unlock(&fe->lock);
			//fm->size--;
			myth_internal_lock_unlock(&fm->lock);
			return;
		}
	}
	//simply ignore
	//assert(0);
}

static inline myth_io_struct_perfd_t myth_fd_map_lookup(myth_fd_map_t fm,int fd)
{//Lookup blocked list from fd, return NULL if lookup fails
	int i;
	myth_fd_map_entry_t fe;
	myth_internal_lock_lock(&fm->lock);
	fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
	myth_internal_lock_lock(&fe->lock);
	for (i=0;i<fe->size;i++){
		if (fe->key[i]==fd){
			assert(fe->data[i]);
			myth_io_struct_perfd_t ret=fe->data[i];
			myth_internal_lock_unlock(&fe->lock);
			myth_internal_lock_unlock(&fm->lock);
			return ret;
		}
	}
	myth_internal_lock_unlock(&fe->lock);
	myth_internal_lock_unlock(&fm->lock);
	return NULL;
}

static inline myth_io_struct_perfd_t myth_fd_map_trylookup(myth_fd_map_t fm,int fd)
{//Lookup blocked list from fd, return NULL if lookup fails
	int i;
	myth_fd_map_entry_t fe;
	if (!myth_internal_lock_trylock(&fm->lock))return NULL;
	fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
	if (!myth_internal_lock_trylock(&fe->lock)){
		myth_internal_lock_unlock(&fm->lock);
		return NULL;
	}
	for (i=0;i<fe->size;i++){
		if (fe->key[i]==fd){
			assert(fe->data[i]);
			myth_io_struct_perfd_t ret=fe->data[i];
			myth_internal_lock_unlock(&fe->lock);
			myth_internal_lock_unlock(&fm->lock);
			return ret;
		}
	}
	myth_internal_lock_unlock(&fe->lock);
	myth_internal_lock_unlock(&fm->lock);
	return NULL;
}

static inline void myth_fd_map_clear(myth_fd_map_t fm)
{//Remove all elements from fd_map
	int i;
	myth_internal_lock_lock(&fm->lock);
	for (i=0;i<MYTH_FD_MAP_SIZE;i++){
		myth_fd_map_entry_t fe;
		fe=&fm->entry[i];
		myth_internal_lock_lock(&fe->lock);
		fe->size=0;
		myth_internal_lock_unlock(&fe->lock);
	}
	//fm->size=0;
	myth_internal_lock_unlock(&fm->lock);
}

static inline void myth_fd_map_destroy(myth_fd_map_t fm)
{
	int i;
	for (i=0;i<MYTH_FD_MAP_SIZE;i++){
		myth_fd_map_entry_t fe;
		fe=&fm->entry[i];
		myth_internal_lock_destroy(&fe->lock);
		myth_free(fe->data,0);
		myth_free(fe->key,0);
	}
	myth_internal_lock_destroy(&fm->lock);
	myth_free(fm->entry,0);
	myth_free(fm,0);
}

static inline void myth_io_wait_list_init(myth_io_wait_list_t wl)
{
	myth_internal_lock_init(&wl->lock);
	wl->count=0;
	wl->size=1;
	assert(real_malloc);
	wl->io_ops=real_malloc(sizeof(myth_io_op_t)*1);
}

static inline void myth_io_wait_list_destroy(myth_io_wait_list_t wl)
{
	myth_internal_lock_destroy(&wl->lock);
	real_free(wl->io_ops);
}

static inline void myth_io_cs_enter(myth_running_env_t e)
{
#ifdef MYTH_USE_IO_THREAD
	int ret;
	ret=real_pthread_mutex_lock(&e->io_struct.mtx);
	if (ret!=0){
		fprintf(stderr,"errno:%d\n",ret);
		errno=ret;
		perror(NULL);
		assert(0);
	}
#endif
#ifdef MYTH_USE_SIGHANDLER
	assert(e->io_struct.cs_flag==0);
	e->io_struct.cs_flag=1;
	myth_wbarrier();
#endif
}

static inline void myth_io_cs_exit(myth_running_env_t e)
{
#ifdef MYTH_USE_SIGHANDLER
	assert(e->io_struct.cs_flag==1);
	myth_wbarrier();
	e->io_struct.cs_flag=0;
#endif
#ifdef MYTH_USE_IO_THREAD
	real_pthread_mutex_unlock(&e->io_struct.mtx);
#endif
}

static inline int myth_io_is_in_cs(myth_running_env_t e)
{
	int ret=0;
#ifdef MYTH_USE_SIGHANDLER
	ret=ret || e->io_struct.cs_flag;
#endif
#ifdef MYTH_USE_IO_THREAD
#endif
	return ret;
}

static inline void myth_io_wait_list_push(myth_running_env_t e,myth_io_wait_list_t wl,myth_io_op_t op)
{
	myth_internal_lock_lock(&wl->lock);
	if (wl->count==wl->size){
		wl->size*=2;
		wl->io_ops=real_realloc(wl->io_ops,sizeof(myth_io_op_t)*wl->size);
	}
	wl->io_ops[wl->count]=op;
	wl->count++;
	myth_internal_lock_unlock(&wl->lock);
}

static inline myth_io_op_t myth_io_wait_list_pop(myth_running_env_t e,myth_io_wait_list_t wl)
{
	myth_io_op_t ret;
#ifdef QUICK_CHECK_IO_WAIT_LIST
	if (!wl->count) return NULL;
#endif
	myth_internal_lock_lock(&wl->lock);
	if (wl->count){
		ret=wl->io_ops[wl->count-1];
		//if (ret)printf("popped:%p->%p\n",wl,ret);
		wl->count--;
	}
	else ret=NULL;
	myth_internal_lock_unlock(&wl->lock);
	//if (ret)printf("unlocked:%p<-%p\n",wl,ret);
	return ret;
}

static inline void myth_io_fd_list_init(myth_running_env_t env,myth_io_fd_list_t cl)
{
	cl->data=myth_flmalloc(env->rank,sizeof(myth_io_struct_perfd_t));
	cl->size=0;
	cl->bufsize=1;
	myth_internal_lock_init(&cl->lock);
}

static inline void myth_io_fd_list_push(myth_running_env_t env,myth_io_fd_list_t cl,myth_io_struct_perfd_t fd_data)
{
	myth_internal_lock_lock(&cl->lock);
	cl->size++;
	if (cl->bufsize<cl->size){
		cl->data=myth_flrealloc(env->rank,cl->bufsize*sizeof(myth_io_struct_perfd_t),cl->data,sizeof(myth_io_struct_perfd_t)*cl->size);
		cl->bufsize=cl->size;
	}
	cl->data[cl->size-1]=fd_data;
	myth_internal_lock_unlock(&cl->lock);
}

static inline myth_io_struct_perfd_t myth_io_fd_list_pop(myth_io_fd_list_t cl)
{
	myth_io_struct_perfd_t ret;
#ifdef QUICK_CHECK_IO_FD_LIST
	if (cl->size<=0) return NULL;
#endif
	myth_internal_lock_lock(&cl->lock);
	if (cl->size>0){
		ret=cl->data[cl->size-1];
		cl->size--;
	}
	else ret=NULL;
	myth_internal_lock_unlock(&cl->lock);
	return ret;
}

static inline myth_io_struct_perfd_t myth_io_fd_list_trypop(myth_io_fd_list_t cl)
{
	myth_io_struct_perfd_t ret;
	if (myth_internal_lock_trylock(&cl->lock)==0)return NULL;
	if (cl->size>0){
		ret=cl->data[cl->size-1];
		cl->size--;
	}
	else ret=NULL;
	myth_internal_lock_unlock(&cl->lock);
	return ret;
}

static inline void myth_io_fd_list_destroy(myth_running_env_t env,myth_io_fd_list_t cl)
{
	myth_flfree(env->rank,cl->bufsize,cl->data);
	cl->size=0;
	cl->bufsize=0;
	myth_internal_lock_destroy(&cl->lock);
}

#endif //MYTH_WRAP_SOCKIO

#endif /* MYTH_IO_STRUCT_H_ */
