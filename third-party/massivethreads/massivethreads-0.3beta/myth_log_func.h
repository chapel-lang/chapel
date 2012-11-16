#ifndef MYTH_LOG_FUNC_H_
#define MYTH_LOG_FUNC_H_

#ifdef MYTH_COLLECT_LOG
extern FILE *g_log_fp;

/*
static inline void myth_log_add_ws(myth_running_env_t env,myth_log_type_t type,int ws_victim)
{
	myth_log_entry_t e;
	if (type!=MYTH_LOG_QUIT && !g_log_worker_stat)return;
	e=&(env->log_data[env->log_count]);
	//store state and tsc to log
	e->tsc=myth_get_rdtsc();
	e->type=type;
	e->ws_victim=ws_victim;
	env->log_count++;
	//Reallocate buffer if buffer is full
	if (env->log_count==env->log_buf_size){
		//myth_log_flush(env);
		int new_log_buf_size=env->log_buf_size*2;
		env->log_data=myth_flrealloc(env->rank,sizeof(myth_log_entry)*env->log_buf_size,env->log_data,sizeof(myth_log_entry)*new_log_buf_size);
		env->log_buf_size=new_log_buf_size;
	}
}

static inline void myth_log_add(myth_running_env_t env,myth_log_type_t type)
{
	myth_log_add_ws(env,type,-1);
}
*/

static inline void myth_log_add(myth_running_env_t env,myth_log_entry_t e)
{
	if (!g_log_worker_stat)return;
	myth_internal_lock_lock(&env->log_lock);
	env->log_data[env->log_count]=*e;
	env->log_data[env->log_count].rank=env->rank;
	env->log_data[env->log_count].tsc=myth_get_rdtsc();
	env->log_count++;
	//Reallocate buffer if buffer is full
	if (env->log_count==env->log_buf_size){
		//myth_log_flush(env);
		int new_log_buf_size=env->log_buf_size*2;
		env->log_data=myth_flrealloc(env->rank,sizeof(myth_log_entry)*env->log_buf_size,env->log_data,sizeof(myth_log_entry)*new_log_buf_size);
		env->log_buf_size=new_log_buf_size;
	}
	myth_internal_lock_unlock(&env->log_lock);
}

static inline void myth_log_add_context_switch(myth_running_env_t env,myth_thread_t th)
{
	//fprintf(stderr,"%lld:%d:%s\n",(long long int)myth_get_rdtsc(),env->rank,(th)?th->annotation_str:"Scheduler");
	myth_log_entry e;
	//store state to log_entry
	e.type=MYTH_LOG_SWITCH;
	e.u.ctx_switch.th=th;
#ifdef MYTH_ENABLE_THREAD_ANNOTATION
	e.u.ctx_switch.recycle_count=(th && th!=TRREAD_PTR_SCHED_SLEEP)?th->recycle_count:0;
#endif
	//strcpy(e->u.str,(th)?th->annotation_str:"Scheduler");
	myth_log_add(env,&e);
}

static inline void myth_log_init(void)
{
	char fname[1000];
	sprintf(fname,"myth-sslog-%d.txt",getpid());
	g_log_fp=fopen(fname,"w");
	g_tsc_base=myth_get_rdtsc();
}

static inline void myth_log_flush_body(void);

static inline void myth_log_fini(void)
{
	myth_log_flush_body();
	int i;
	for (i=0;i<g_worker_thread_num;i++){
		myth_internal_lock_destroy(&g_envs[i].log_lock);
	}
}
static inline void myth_log_worker_init(myth_running_env_t env)
{
	env->log_buf_size=MYTH_LOG_INITIAL_BUFFER_SIZE;
	env->log_data=myth_flmalloc(env->rank,sizeof(myth_log_entry)*env->log_buf_size);
	env->log_count=0;
	myth_internal_lock_init(&env->log_lock);
}
static inline void myth_log_worker_fini(myth_running_env_t env)
{
	//Add dummy context switch event
	//myth_log_add_context_switch(env,NULL);
}
static inline void myth_log_annotate_thread_body(myth_thread_t th,char *name)
{
#ifdef MYTH_ENABLE_THREAD_ANNOTATION
	myth_log_entry e;
	e.type=MYTH_LOG_THREAD_ANNOTATION;
	e.u.annotation.th=th;
	e.u.annotation.recycle_count=(th)?th->recycle_count:0;
	strncpy(e.u.annotation.str,name,MYTH_THREAD_ANNOTATION_MAXLEN-1);
	myth_log_add(myth_get_current_env(),&e);
#endif
}

static inline void myth_log_get_thread_annotation(myth_log_entry_t logs,int logcount,myth_thread_t th,int recycle_count,char *ret)
{
	if (!th){
		strcpy(ret,"Scheduler");
		return;
	}
	if (th==TRREAD_PTR_SCHED_SLEEP){
		strcpy(ret,"Sleep");
		return;
	}
#ifdef MYTH_ENABLE_THREAD_ANNOTATION
	int i;
	for (i=logcount-1;i>=0;i--){
		assert(logs[i].type==MYTH_LOG_THREAD_ANNOTATION);
		if (logs[i].u.annotation.th==th && logs[i].u.annotation.recycle_count==recycle_count){
			strcpy(ret,logs[i].u.annotation.str);
			return;
		}
	}
	sprintf(ret,"%p@%d",th,recycle_count);
#else
	strcpy(ret,"Computation");
#endif
}

/*
static inline void myth_log_get_thread_annotation_body(myth_thread_t th,char *name)
{
#ifdef MYTH_ENABLE_THREAD_ANNOTATION
	myth_internal_lock_lock(&th->lock);
	strcpy(name,th->annotation_str);
	myth_internal_lock_unlock(&th->lock);
#else
	strcpy(name,"");
#endif
}
*/

static inline void myth_log_start_body(void)
{
	g_log_worker_stat=1;
}

static inline void myth_log_pause_body(void)
{
	g_log_worker_stat=0;
}

int myth_log_entry_compare(const void *pa,const void *pb);

static inline void myth_log_reset_body(void)
{
	int i,j;
	for (i=0;i<g_worker_thread_num;i++){
		myth_running_env_t e=&g_envs[i];
		myth_internal_lock_lock(&e->log_lock);
		qsort(e->log_data,e->log_count,sizeof(myth_log_entry),myth_log_entry_compare);
		int n_annotation=0;
		for (j=0;j<e->log_count;j++){
			if (e->log_data[j].type!=MYTH_LOG_THREAD_ANNOTATION){
				break;
			}
		}
		n_annotation=j;
		e->log_count=n_annotation;
		myth_internal_lock_unlock(&e->log_lock);
	}
}

static inline void myth_log_flush_body(void)
{
	//TODO:emit category
	//TODO:emit data
	//merge all the logs
	int i;
	int total_log_entry_count=0;
	for (i=0;i<g_worker_thread_num;i++){
		myth_internal_lock_lock(&g_envs[i].log_lock);
		total_log_entry_count+=g_envs[i].log_count;
	}
	myth_log_entry_t all_logs=real_malloc(sizeof(myth_log_entry)*total_log_entry_count);
	int pos=0;
	for (i=0;i<g_worker_thread_num;i++){
		memcpy(&all_logs[pos],g_envs[i].log_data,sizeof(myth_log_entry)*g_envs[i].log_count);
		pos+=g_envs[i].log_count;
		myth_internal_lock_unlock(&g_envs[i].log_lock);
	}
	//sort by type and timestamp
	qsort(all_logs,total_log_entry_count,sizeof(myth_log_entry),myth_log_entry_compare);
	int n_annotation=0;
	int n_switch=0;
	//fprintf(stderr,"total=%d,annotation=%d,switch=%d\n",total_log_entry_count,n_annotation,n_switch);
	for (i=0;i<total_log_entry_count;i++){
		if (all_logs[i].type!=MYTH_LOG_THREAD_ANNOTATION){
			break;
		}
	}
	n_annotation=i;
	for (;i<total_log_entry_count;i++){
		if (all_logs[i].type!=MYTH_LOG_SWITCH){
			break;
		}
	}
	n_switch=i-n_annotation;
	int wth_idx=0;
	for (i=n_annotation;i<n_switch+n_annotation-1;i++){
		myth_log_entry_t le=&all_logs[i],le2=&all_logs[i+1];
		assert(le->type==MYTH_LOG_SWITCH && le2->type==MYTH_LOG_SWITCH);
		assert(wth_idx>=le->rank);
		if (le->u.ctx_switch.th==THREAD_PTR_SCHED_TERM)wth_idx++;
		if (wth_idx>le->rank){
			continue;
		}
		char th_name[100];
		int rank=le->rank;
		uint64_t t0,t1;
		myth_log_get_thread_annotation(all_logs,n_annotation,le->u.ctx_switch.th,le->u.ctx_switch.recycle_count,th_name);
		t0=le->tsc-g_tsc_base;
		while (1){
#ifdef MYTH_LOG_MERGE_SAME_NAME_THREADS
			char n[100];
			myth_log_get_thread_annotation(all_logs,n_annotation,le2->u.ctx_switch.th,le2->u.ctx_switch.recycle_count,n);
#endif
			if (rank!=le2->rank ||
				le2->u.ctx_switch.th==THREAD_PTR_SCHED_TERM ||
#ifdef MYTH_LOG_MERGE_SAME_NAME_THREADS
				strcmp(th_name,n)!=0 ||
#endif
				i==n_switch+n_annotation-1-1){
				break;
			}
			i++;
			le2=&all_logs[i+1];
		}
		t1=le2->tsc-1-g_tsc_base;
		if (rank==le2->rank){
			//fprintf(g_log_fp,"%s,%lld,%lld,%d,%s\n",th_name,(long long int)t0,(long long int)t1,le->rank,th_name);
			fprintf(g_log_fp,"%s,%.9lf,%.9lf,%d,%s\n",th_name,(double)t0/(1000.0*1000.0*1000.0),(double)t1/(1000.0*1000.0*1000.0),le->rank,th_name);
		}
	}
	fflush(g_log_fp);
	//clear log buffer
	myth_log_reset_body();
#if 0
	int i,j;
	myth_textlog_entry_t tx_logs;
	int textlog_count=0;
	int textlog_size=1;
	uint64_t min_ts=0xFFFFFFFFFFFFFFFFULL;
	uint64_t *idle_sum,*user_sum,idle_sum_all,user_sum_all;
	uint64_t *ws_count,ws_count_all;
#ifdef MYTH_LOG_EMIT_TEXTLOG
	//Emit category
	fprintf(g_log_fp,"Category[ index=0 name=Work_Stealing topo=Arrow color=(255,255,255,255,true) width=3 ]\n");
	fprintf(g_log_fp,"Category[ index=181 name=User_Computation topo=State color=(255,0,0,127,true) width=1 ]\n");
	fprintf(g_log_fp,"Category[ index=182 name=Thread_Idle topo=State color=(0,0,255,127,true) width=1 ]\n");
#endif
	tx_logs=malloc(sizeof(myth_textlog_entry)*textlog_size);
	idle_sum=malloc(sizeof(uint64_t)*g_worker_thread_num);
	user_sum=malloc(sizeof(uint64_t)*g_worker_thread_num);
	ws_count=malloc(sizeof(uint64_t)*g_worker_thread_num);
	//Merge all the logs and sort by time
	for (i=0;i<g_worker_thread_num;i++){
		int log_num;
		myth_log_entry_t e;
		idle_sum[i]=0;user_sum[i]=0;ws_count[i]=0;
		log_num=g_envs[i].log_count;
		e=g_envs[i].log_data;
		for (j=0;j<log_num-1;j++){
			myth_textlog_entry_t txe;
			uint64_t ts,te;
			int cat;
			txe=&tx_logs[textlog_count];
			ts=e[j].tsc;
			te=e[j+1].tsc;
			if (min_ts>ts)min_ts=ts;
			switch (e[j].type)
			{
			case MYTH_LOG_USER:
				if (e[j].ws_victim>0){
					ws_count[i]++;
					txe->ts=ts;
					txe->te=ts;
					txe->cat=0;
					txe->id_a=e[j].ws_victim;
					txe->id_b=i;
					//fprintf(g_log_fp,"Primitive[ TimeBBox(%llu,%llu) Category=0 (%llu, %d) (%llu, %d) ]\n",
					//					(unsigned long long)ts,(unsigned long long)ts,(unsigned long long)ts,e[j].ws_victim,(unsigned long long)ts,i);
					textlog_count++;
					if (textlog_count==textlog_size){
						textlog_size*=2;
						tx_logs=realloc(tx_logs,sizeof(myth_textlog_entry)*textlog_size);
					}
					txe=&tx_logs[textlog_count];
				}
				user_sum[i]+=te-ts;
				cat=181;
				break;
			case MYTH_LOG_WS:
				idle_sum[i]+=te-ts;
				cat=182;
				break;
			default:
				assert(0);
				break;
			}
			txe->ts=ts;
			txe->te=te;
			txe->cat=cat;
			txe->id_a=i;
			txe->id_b=i;
			//fprintf(g_log_fp,"Primitive[ TimeBBox(%llu,%llu) Category=%d (%llu, %d) (%llu, %d) ]\n",
			//		(unsigned long long)ts,(unsigned long long)te,cat,(unsigned long long)ts,i,(unsigned long long)te,i);
			textlog_count++;
			if (textlog_count==textlog_size){
				textlog_size*=2;
				tx_logs=realloc(tx_logs,sizeof(myth_textlog_entry)*textlog_size);
			}
		}
		assert(e[j].type==MYTH_LOG_QUIT);
	}
	qsort(tx_logs,textlog_count,sizeof(myth_textlog_entry),myth_textlog_entry_compare);
	//Emit logs
	for (i=0;i<textlog_count;i++){
		tx_logs[i].te-=min_ts;
		tx_logs[i].ts-=min_ts;
	}
#ifdef MYTH_LOG_EMIT_TEXTLOG
	for (i=0;i<textlog_count;i++){
		fprintf(g_log_fp,"Primitive[ TimeBBox(%llu,%llu) Category=%d (%llu, %d) (%llu, %d) ]\n",
				(unsigned long long)tx_logs[i].ts,(unsigned long long)tx_logs[i].te,
				tx_logs[i].cat,(unsigned long long)tx_logs[i].ts,
				tx_logs[i].id_a,(unsigned long long)tx_logs[i].te,tx_logs[i].id_b);
	}
	free(tx_logs);
	fclose(g_log_fp);
#endif
	idle_sum_all=user_sum_all=ws_count_all=0;
	for (i=0;i<g_worker_thread_num;i++){
#ifdef MYTH_LOG_EMIT_STAT_WORKER
		FILE *fp_stat_out;
		fp_stat_out=stdout;
		fprintf(fp_stat_out,"Worker %d : %llu,%llu,%lf (%llu Work-Stealing)\n",i,(unsigned long long)user_sum[i],(unsigned long long)idle_sum[i],(double)user_sum[i]/(double)(idle_sum[i]+user_sum[i]),(unsigned long long)ws_count[i]);
#endif
		idle_sum_all+=idle_sum[i];user_sum_all+=user_sum[i];ws_count_all+=ws_count[i];
	}
#ifdef MYTH_LOG_EMIT_STAT_ALL
	FILE *fp_stat_out;
	fp_stat_out=stdout;
	fprintf(fp_stat_out,"Total work-stealing count : %llu ( %lf per core)\n",(unsigned long long)ws_count_all,(double)ws_count_all/(double)g_worker_thread_num);
	fprintf(fp_stat_out,"Total user time : %llu ( %lf per core)\n",(unsigned long long)user_sum_all,(double)user_sum_all/(double)g_worker_thread_num);
	fprintf(fp_stat_out,"Total idle time : %llu ( %lf per core)\n",(unsigned long long)idle_sum_all,(double)idle_sum_all/(double)g_worker_thread_num);
#endif
	free(idle_sum);
	free(user_sum);
	free(ws_count);
	/*
	for (i=0;i<g_worker_thread_num;i++){
		myth_flfree(g_envs[i].rank,sizeof(myth_log_entry)*g_envs[i].log_buf_size,g_envs[i].log_data);
	}*/
#endif
}

#else
static inline void myth_log_add_ws(myth_running_env_t env,myth_log_type_t type,int ws_victim)
{
}
static inline void myth_log_add(myth_running_env_t env,myth_log_type_t type)
{
}
static inline void myth_log_add_context_switch(myth_running_env_t env,myth_thread_t th)
{

}
static inline void myth_log_init(void)
{
}
static inline void myth_log_fini(void)
{
}
static inline void myth_log_worker_init(myth_running_env_t env)
{
}
static inline void myth_log_worker_fini(myth_running_env_t env)
{
}
static inline void myth_log_annotate_thread_body(myth_thread_t th,char *name)
{
}
static inline void myth_log_get_thread_annotation_body(myth_thread_t th,char *name)
{
}
static inline void myth_log_start_body(void)
{
}
static inline void myth_log_pause_body(void)
{
}
static inline void myth_log_reset_body(void)
{
}
static inline void myth_log_flush_body(void)
{
}
#endif


#endif /* MYTH_LOG_FUNC_H_ */
