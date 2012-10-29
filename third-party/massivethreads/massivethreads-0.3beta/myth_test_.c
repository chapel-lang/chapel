#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
//myth:MY THread library
//mythのテスト

#include "myth.h"

void *myth_test_thread(void *args)
{
	int i;
	for (i=0;i<10;i++){
		fprintf(stderr,"fprintf %d from Thread %p\n",i,args)
		;
	}
	//while (1){myth_yield();};
	return args;
}

int main()
{
#define DO_PASS1
#define DO_PASS2
#define DO_PASS3
#ifdef DO_PASS1
	{
		fprintf(stderr,"Pass 1:initialize and finalize\n");
		myth_init();//スケジューラ起動
		myth_fini();//スケジューラ終了
	}
#endif
#ifdef DO_PASS2
	{
		int i;
		fprintf(stderr,"Pass 2:yield\n");
		myth_init();
		for (i=0;i<10;i++)
			myth_yield();//一度スケジューラに制御を返す
		myth_fini();
	}
#endif
#ifdef DO_PASS3
	{
		fprintf(stderr,"Pass 3:create and destroy\n");
		myth_init();
		myth_sched_prof_start();
		srand(time(NULL));
		void *a,*b;
		a=(void*)(intptr_t)rand();
		b=(void*)(intptr_t)rand();
		myth_thread_t th;
		//fprintf(stderr,"argument:%p\n",a);
		th=myth_create(myth_test_thread,a);
		myth_test_thread(b);
		myth_yield();
		myth_join(th,&b);
		//fprintf(stderr,"return value:%p\n",b);
		assert(a==b);
		myth_fini();
	}
#endif
	fprintf(stderr,"MYTH library has passed all tests\n");
	return 0;
}
