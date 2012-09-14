#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "chplrt.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "error.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

// Helper functions

static int mysystem(const char* command, const char* description, 
                    int ignorestatus) {
  int status = system(command);

  if (status == -1) {
    chpl_error("system() fork failed", 0, "(command-line)");
  } else if (status != 0 && !ignorestatus) {
    chpl_error(description, 0, "(command-line)");
  }

  return status;
}

// Chapel interface

int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_numLocales = 1;
  chpl_localeID = 0;
}

void chpl_comm_post_mem_init(void) { }

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  int i;
  char* command = chpl_glom_strings(2, "gdb -q -ex 'break gdbShouldBreakHere' --args ",
                                    argv[0]);
  for (i=1; i<argc; i++) {
    if (i != gdbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = mysystem(command, "running gdb", 0);

  return 1;
}

void chpl_comm_post_task_init(void) { }

void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on a single locale\n");
}

void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
  *start_p = NULL;
  *size_p  = 0;
}

void chpl_comm_alloc_registry(int numGlobals) { 
  chpl_globals_registry = chpl_globals_registry_static;
}

void chpl_comm_broadcast_global_vars(int numGlobals) { }

void chpl_comm_broadcast_private(int id, int32_t sizee, int32_t tid) { }

void chpl_comm_barrier(const char *msg) { }

void chpl_comm_pre_task_exit(int all) { }

void chpl_comm_exit(int all, int status) { }

void  chpl_comm_put(void* addr, int32_t locale, void* raddr,
                    int32_t size, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  memcpy(raddr, addr, size*len);
}

void  chpl_comm_get(void* addr, int32_t locale, void* raddr,
                    int32_t size, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  memcpy(addr, raddr, size*len);
}

void  chpl_comm_puts(void* dstaddr, void* dststrides, int32_t dstlocale, 
		     void* srcaddr, void* srcstrides, void* count,
		     int32_t stridelevels, int32_t elemSize, int32_t typeIndex, 
		     int ln, chpl_string fn)
{
  const size_t strlvls=(size_t)stridelevels;
  int i,t,j,total;
  size_t *dststr=(size_t*)chpl_mem_allocMany(strlvls,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0); 
  size_t *srcstr=(size_t*)chpl_mem_allocMany(strlvls,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0); 
  size_t *cnt=(size_t*)chpl_mem_allocMany(strlvls+1,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_COUNTS,0,0); 
  
  //Only count[0] and strides are meassured in number of bytes.
  cnt[0]=((int32_t*)count)[0] * elemSize;
  srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
  dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
  for (i=1;i<strlvls;i++)
    { 
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i]=((int32_t*)count)[i];
    }
  cnt[strlvls]=((int32_t*)count)[strlvls];
  
  //Number of memcpy operations to do
  total=1;
  for (i=0;i<=stridelevels-1;i++)
    total=total*cnt[i+1];
  
  int *carry=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  int *x=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  
  //displacement from the dstaddr and srcaddr start points
  int *srcdesp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  int *dstdesp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  
  for (j=0;j<total;j++)
    {     
      carry[j]=1;
      for (t=1;t<=stridelevels;t++)
	{
	  if (cnt[t]*carry[j]>=j+1) //IF 1
	    {
	      x[j]=j/carry[j];
	      int off =j-(carry[j]*x[j]);
        
	      if (carry[j]!=1) //IF 2
		{
		  srcdesp[j]=srcstr[t-1]*x[j]+srcdesp[off];
		  dstdesp[j]=dststr[t-1]*x[j]+dstdesp[off];
		}
	      else //ELSE 2
		{
		  srcdesp[j]=srcstr[t-1]*x[j];
		  dstdesp[j]=dststr[t-1]*x[j];
		}   
	      memcpy(srcaddr+srcdesp[j],dstaddr+dstdesp[j],cnt[0]);
	      break;
	    }
	  else //ELSE 1
	    carry[j]=carry[j]*cnt[t];
	}
    }
  chpl_mem_free(carry,0,0);
  chpl_mem_free(x,0,0);
  chpl_mem_free(srcdesp,0,0);
  chpl_mem_free(dstdesp,0,0); 
}

void  chpl_comm_gets(void* dstaddr, void* dststrides, int32_t srclocale, 
		     void* srcaddr, void* srcstrides, void* count,
		     int32_t stridelevels, int32_t elemSize, int32_t typeIndex, 
		     int ln, chpl_string fn)
{
  const size_t strlvls=(size_t)stridelevels;
  int i,t,j,total;
  size_t *dststr=(size_t*)chpl_mem_allocMany(strlvls,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0); 
  size_t *srcstr=(size_t*)chpl_mem_allocMany(strlvls,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0); 
  size_t *cnt=(size_t*)chpl_mem_allocMany(strlvls+1,sizeof(size_t),CHPL_RT_MD_GETS_PUTS_COUNTS,0,0); 
  
  //Only count[0] and strides are meassured in number of bytes.
  cnt[0]=((int32_t*)count)[0] * elemSize;
  srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
  dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
  for (i=1;i<strlvls;i++)
    { 
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i]=((int32_t*)count)[i];
    }
  cnt[strlvls]=((int32_t*)count)[strlvls];
  
  //Number of memcpy operations to do
  total=1;
  for (i=0;i<=stridelevels-1;i++)
    {
      total=total*cnt[i+1];
    }

  int *carry=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  int *x=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  
  //displacement from the dstaddr and srcaddr start points
  int *srcdesp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  int *dstdesp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
  
  for (j=0;j<total;j++)
    {     
      carry[j]=1;
      for (t=1;t<=stridelevels;t++)
	{
	  if (cnt[t]*carry[j]>=j+1) //IF 1
	    {
	      x[j]=j/carry[j];
	      int off =j-(carry[j]*x[j]);
        
	      if (carry[j]!=1) //IF 2
		{
		  srcdesp[j]=srcstr[t-1]*x[j]+srcdesp[off];
		  dstdesp[j]=dststr[t-1]*x[j]+dstdesp[off];
		}
	      else //ELSE 2
		{
		  srcdesp[j]=srcstr[t-1]*x[j];
		  dstdesp[j]=dststr[t-1]*x[j];
		}   
	      memcpy(dstaddr+dstdesp[j], srcaddr+srcdesp[j], cnt[0]);
	      break;
	    }
	  else //ELSE 1
	    carry[j]=carry[j]*cnt[t];
	}
    }
  chpl_mem_free(carry,0,0);
  chpl_mem_free(x,0,0);
  chpl_mem_free(srcdesp,0,0);
  chpl_mem_free(dstdesp,0,0);
}

typedef struct {
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

static void fork_nb_wrapper(fork_t* f) {
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  chpl_mem_free(f, 0, 0);
}

void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg,
                       int32_t arg_size, int32_t arg_tid) {
  fork_t *info;
  int     info_size;

  info_size = sizeof(fork_t) + arg_size;
  info = (fork_t*)chpl_mem_allocMany(info_size, sizeof(char), CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
  info->fid = fid;
  info->arg_size = arg_size;
  if (arg_size)
    memcpy(&(info->arg), arg, arg_size);
  chpl_task_begin((chpl_fn_p)fork_nb_wrapper, (void*)info, false, false, NULL);
}

void chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg,
                    int32_t arg_size, int32_t arg_tid) {
  (*chpl_ftable[fid])(arg);
}

// Same as chpl_comm_fork()
void chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg,
                         int32_t arg_size, int32_t arg_tid) {
  (*chpl_ftable[fid])(arg);
}

int chpl_comm_numPollingTasks(void) { return 0; }

void chpl_startVerboseComm() { }
void chpl_stopVerboseComm() { }
void chpl_startVerboseCommHere() { }
void chpl_stopVerboseCommHere() { }

void chpl_startCommDiagnostics() { }
void chpl_stopCommDiagnostics() { }
void chpl_startCommDiagnosticsHere() { }
void chpl_stopCommDiagnosticsHere() { }

void chpl_resetCommDiagnosticsHere() { }
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) { }

uint64_t chpl_numCommGets(void) { return 0; }
uint64_t chpl_numCommNBGets(void) { return 0; }
uint64_t chpl_numCommTestNBGets(void) { return 0; }
uint64_t chpl_numCommWaitNBGets(void) { return 0; }
uint64_t chpl_numCommPuts(void) { return 0; }
uint64_t chpl_numCommForks(void) { return 0; }
uint64_t chpl_numCommFastForks(void) { return 0; }
uint64_t chpl_numCommNBForks(void) { return 0; }
