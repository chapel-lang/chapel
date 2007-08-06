/* $Id$ */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
#  include <windows.h>
#  define sleep(x) Sleep(1000*(x))
#else
#  include <unistd.h>
#endif

/* ARMCI is impartial to message-passing libs - we handle them with MP macros */
#if defined(PVM)
#   include <pvm3.h>
#   ifdef CRAY
#     define MPGROUP         (char *)NULL
#     define MP_INIT(arc,argv)
#   else
#     define MPGROUP           "mp_working_group"
#     define MP_INIT(arc,argv) pvm_init(arc, argv)
#   endif
#   define MP_FINALIZE()     pvm_exit()
#   define MP_BARRIER()      pvm_barrier(MPGROUP,-1)
#   define MP_MYID(pid)      *(pid)   = pvm_getinst(MPGROUP,pvm_mytid())
#   define MP_PROCS(pproc)   *(pproc) = (int)pvm_gsize(MPGROUP)
    void pvm_init(int argc, char *argv[]);
#elif defined(TCGMSG)
#   include <sndrcv.h>
    long tcg_tag =30000;
#   define MP_BARRIER()      SYNCH_(&tcg_tag)
#   define MP_INIT(arc,argv) PBEGIN_((argc),(argv))
#   define MP_FINALIZE()     PEND_()
#   define MP_MYID(pid)      *(pid)   = (int)NODEID_()
#   define MP_PROCS(pproc)   *(pproc) = (int)NNODES_()
#elif defined(BGML)
# include "bgml.h"
# define MP_BARRIER()      bgml_barrier(3);
# define MP_FINALIZE()
# define MP_INIT(arc, argv)
# define MP_MYID(pid)      *(pid)=BGML_Messager_rank();
# define MP_PROCS(pproc)   *(pproc)=BGML_Messager_size();
#else
#   include <mpi.h>
#   define MP_BARRIER()      MPI_Barrier(MPI_COMM_WORLD)
#   define MP_FINALIZE()     MPI_Finalize()
#   define MP_INIT(arc,argv) MPI_Init(&(argc),&(argv))
#   define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
#   define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#endif

#include "armci.h"

#define DIM1 5
#define DIM2 3
#ifdef __sun
/* Solaris has shared memory shortages in the default system configuration */
# define DIM3 6
# define DIM4 5
# define DIM5 4
#elif defined(__alpha__)
# define DIM3 8
# define DIM4 5
# define DIM5 6
#else
# define DIM3 8
# define DIM4 9
# define DIM5 7
#endif
#define DIM6 3
#define DIM7 2


#define OFF 1
#define EDIM1 (DIM1+OFF)
#define EDIM2 (DIM2+OFF)
#define EDIM3 (DIM3+OFF)
#define EDIM4 (DIM4+OFF)
#define EDIM5 (DIM5+OFF)
#define EDIM6 (DIM6+OFF)
#define EDIM7 (DIM7+OFF)

#define DIMS 4
#define MAXDIMS 7
#define MAX_DIM_VAL 50 
#define LOOP 200

#define BASE 100.
#define MAXPROC 128
#define TIMES 100

#ifdef CRAY
# define ELEMS 800
#else
# define ELEMS 200
#endif


/***************************** macros ************************/
#define COPY(src, dst, bytes) memcpy((dst),(src),(bytes))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) <0) ? -(a) : (a))

/***************************** global data *******************/
int me, nproc;
void* work[MAXPROC]; /* work array for propagating addresses */



#ifdef PVM
void pvm_init(int argc, char *argv[])
{
    int mytid, mygid, ctid[MAXPROC];
    int np, i;

    mytid = pvm_mytid();
    if((argc != 2) && (argc != 1)) goto usage;
    if(argc == 1) np = 1;
    if(argc == 2)
        if((np = atoi(argv[1])) < 1) goto usage;
    if(np > MAXPROC) goto usage;

    mygid = pvm_joingroup(MPGROUP);

    if(np > 1)
        if (mygid == 0) 
            i = pvm_spawn(argv[0], argv+1, 0, "", np-1, ctid);

    while(pvm_gsize(MPGROUP) < np) sleep(1);

    /* sync */
    pvm_barrier(MPGROUP, np);
    
    printf("PVM initialization done!\n");
    
    return;

usage:
    fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
    pvm_exit();
    exit(-1);
}
#endif

/*\ generate random range for a section of multidimensional array 
\*/
void get_range(int ndim, int dims[], int lo[], int hi[]) 
{
	int dim;
	for(dim=0; dim <ndim;dim++){
		int toss1, toss2;
		toss1 = rand()%dims[dim];
		toss2 = rand()%dims[dim];
		if(toss1<toss2){
			lo[dim]=toss1;
			hi[dim]=toss2;
		}else {
  			hi[dim]=toss1;
			lo[dim]=toss2;
		}
    }
}



/*\ generates a new random range similar to the input range for an array with specified dimensions
\*/
void new_range(int ndim, int dims[], int lo[], int hi[],int new_lo[], int new_hi[])
{
	int dim;
	for(dim=0; dim <ndim;dim++){
		int toss, range;
		int diff = hi[dim] -lo[dim]+1;
		assert(diff <= dims[dim]);
                range = dims[dim]-diff;
                toss = (range > 0)? rand()%range : lo[dim];
		new_lo[dim] = toss;
		new_hi[dim] = toss + diff -1;
		assert(new_hi[dim] < dims[dim]);
		assert(diff == (new_hi[dim] -new_lo[dim]+1));
	}
}





/*\ print range of ndim dimensional array with two strings before and after
\*/
void print_range(char *pre,int ndim, int lo[], int hi[], char* post)
{
	int i;

	printf("%s[",pre);
	for(i=0;i<ndim;i++){
		printf("%d:%d",lo[i],hi[i]);
		if(i==ndim-1)printf("] %s",post);
		else printf(",");
	}
}

/*\ print subscript of ndim dimensional array with two strings before and after
\*/
void print_subscript(char *pre,int ndim, int subscript[], char* post)
{
	int i;

	printf("%s [",pre);
	for(i=0;i<ndim;i++){
		printf("%d",subscript[i]);
		if(i==ndim-1)printf("] %s",post);
		else printf(",");
	}
}


/*\ print a section of a 2-D array of doubles
\*/
void print_2D_double(double *a, int ld, int *lo, int *hi)
{
int i,j;
     for(i=lo[0];i<=hi[0];i++){
       for(j=lo[1];j<=hi[1];j++) printf("%13f ",a[ld*j+i]);
       printf("\n");
     }
}
          

/*\ initialize array: a[i,j,k,..]=i+100*j+10000*k+ ... 
\*/
void init(double *a, int ndim, int elems, int dims[])
{
  int idx[MAXDIMS];
  int i,dim;

 	for(i=0; i<elems; i++){
		int Index = i;
		double field, val;
        
		for(dim = 0; dim < ndim; dim++){
			idx[dim] = Index%dims[dim];
			Index /= dims[dim];
		}
		
        field=1.; val=0.;
		for(dim=0; dim< ndim;dim++){
			val += field*idx[dim];
			field *= BASE;
		}
		a[i] = val;
		/* printf("(%d,%d,%d)=%6.0f",idx[0],idx[1],idx[2],val); */
	}
}


/*\ compute Index from subscript
 *  assume that first subscript component changes first
\*/
int Index(int ndim, int subscript[], int dims[])
{
	int idx = 0, i, factor=1;
	for(i=0;i<ndim;i++){
		idx += subscript[i]*factor;
		factor *= dims[i];
	}
	return idx;
}


void update_subscript(int ndim, int subscript[], int lo[], int hi[], int dims[])
{
	int i;
	for(i=0;i<ndim;i++){
		if(subscript[i] < hi[i]) { subscript[i]++; return; }
		subscript[i] = lo[i];
	}
}

	

void compare_patches(double eps, int ndim, double *patch1, int lo1[], int hi1[],
                     int dims1[],double *patch2, int lo2[], int hi2[], 
                     int dims2[])
					           
{
	int i,j, elems=1;	
	int subscr1[MAXDIMS], subscr2[MAXDIMS];
        double diff,max;

	for(i=0;i<ndim;i++){   /* count # of elements & verify consistency of both patches */
		int diff = hi1[i]-lo1[i];
		assert(diff == (hi2[i]-lo2[i]));
		assert(diff < dims1[i]);
		assert(diff < dims2[i]);
		elems *= diff+1;
		subscr1[i]= lo1[i];
		subscr2[i]=lo2[i];
	}

	
	/* compare element values in both patches */ 
	for(j=0; j< elems; j++){ 
		int idx1, idx2, offset1, offset2;
		
		idx1 = Index(ndim, subscr1, dims1);	 /* calculate element Index from a subscript */
		idx2 = Index(ndim, subscr2, dims2);

		if(j==0){
			offset1 =idx1;
			offset2 =idx2;
		}
		idx1 -= offset1;
		idx2 -= offset2;
		

                diff = patch1[idx1] - patch2[idx2];
                max  = MAX(ABS(patch1[idx1]),ABS(patch2[idx2]));
                if(max == 0. || max <eps) max = 1.; 

		if(eps < ABS(diff)/max){
			char msg[48];
			sprintf(msg,"(proc=%d):%f",me,patch1[idx1]);
			print_subscript("ERROR: a",ndim,subscr1,msg);
			sprintf(msg,"%f\n",patch2[idx2]);
			print_subscript(" b",ndim,subscr2,msg);
                        fflush(stdout);
                        sleep(1);
                        ARMCI_Error("Bailing out",0);
		}

		{ /* update subscript for the patches */
		   update_subscript(ndim, subscr1, lo1,hi1, dims1);
		   update_subscript(ndim, subscr2, lo2,hi2, dims2);
		}
	}
				 
	

	/* make sure we reached upper limit */
	/*for(i=0;i<ndim;i++){ 
		assert(subscr1[i]==hi1[i]);
		assert(subscr2[i]==hi2[i]);
	}*/ 
}


void scale_patch(double alpha, int ndim, double *patch1, int lo1[], int hi1[], int dims1[])
{
	int i,j, elems=1;	
	int subscr1[MAXDIMS];

	for(i=0;i<ndim;i++){   /* count # of elements in patch */
		int diff = hi1[i]-lo1[i];
		assert(diff < dims1[i]);
		elems *= diff+1;
		subscr1[i]= lo1[i];
	}

	/* scale element values in both patches */ 
	for(j=0; j< elems; j++){ 
		int idx1, offset1;
		
		idx1 = Index(ndim, subscr1, dims1);	 /* calculate element Index from a subscript */

		if(j==0){
			offset1 =idx1;
		}
		idx1 -= offset1;

		patch1[idx1] *= alpha;
		update_subscript(ndim, subscr1, lo1,hi1, dims1);
	}	
}


void create_array(void *a[], int elem_size, int ndim, int dims[])
{
     int bytes=elem_size, i, rc;

     assert(ndim<=MAXDIMS);
     for(i=0;i<ndim;i++)bytes*=dims[i];

     rc = ARMCI_Malloc(a, bytes);
     assert(rc==0);
     
     assert(a[me]);
     
}

void destroy_array(void *ptr[])
{
    MP_BARRIER();

    assert(!ARMCI_Free(ptr[me]));
}


int loA[MAXDIMS], hiA[MAXDIMS];
int dimsA[MAXDIMS]={DIM1,DIM2,DIM3,DIM4,DIM5,DIM6, DIM7};
int loB[MAXDIMS], hiB[MAXDIMS];
int dimsB[MAXDIMS]={EDIM1,EDIM2,EDIM3,EDIM4,EDIM5,EDIM6,EDIM7};
int count[MAXDIMS];
int strideA[MAXDIMS], strideB[MAXDIMS];
int loC[MAXDIMS], hiC[MAXDIMS];
int idx[MAXDIMS]={0,0,0,0,0,0,0};

    
void test_dim(int ndim)
{
	int dim,elems;
	int i,j, proc;
	/* double a[DIM4][DIM3][DIM2][DIM1], b[EDIM4][EDIM3][EDIM2][EDIM1];*/
        void *b[MAXPROC];
        void *a, *c;

	elems = 1;   
        strideA[0]=sizeof(double); 
        strideB[0]=sizeof(double);
	for(i=0;i<ndim;i++){
		strideA[i] *= dimsA[i];
		strideB[i] *= dimsB[i];
                if(i<ndim-1){
                     strideA[i+1] = strideA[i];
                     strideB[i+1] = strideB[i];
                }
		elems *= dimsA[i];
	}

        /* create shared and local arrays */
        create_array(b, sizeof(double),ndim,dimsB);
        a = malloc(sizeof(double)*elems);
        assert(a);
        c = malloc(sizeof(double)*elems);
        assert(c);

	init(a, ndim, elems, dimsA);
	
	if(me==0){
            printf("--------array[%d",dimsA[0]);
	    for(dim=1;dim<ndim;dim++)printf(",%d",dimsA[dim]);
	    printf("]--------\n");
        }
        sleep(1);

        ARMCI_AllFence();
        MP_BARRIER();
	for(i=0;i<LOOP;i++){
	    int idx1, idx2, idx3;
	    get_range(ndim, dimsA, loA, hiA);
	    new_range(ndim, dimsB, loA, hiA, loB, hiB);
	    new_range(ndim, dimsA, loA, hiA, loC, hiC);

            proc=nproc-1-me;

            if(me==0){
	       print_range("local",ndim,loA, hiA,"-> ");
	       print_range("remote",ndim,loB, hiB,"-> ");
	       print_range("local",ndim,loC, hiC,"\n");
            }

	    idx1 = Index(ndim, loA, dimsA);
	    idx2 = Index(ndim, loB, dimsB);
	    idx3 = Index(ndim, loC, dimsA);

	    for(j=0;j<ndim;j++)count[j]=hiA[j]-loA[j]+1;

	    count[0]   *= sizeof(double); /* convert range to bytes at stride level zero */

            (void)ARMCI_PutS((double*)a + idx1, strideA, (double*)b[proc] + idx2, strideB, count, ndim-1, proc);

/*            sleep(1);*/

/*            printf("%d: a=(%x,%f) b=(%x,%f)\n",me,idx1 + (double*)a,*(idx1 + (double*)a),idx2 + (double*)b,*(idx2 + (double*)b));*/
/*            fflush(stdout);*/
/*            sleep(1);*/

            /* note that we do not need ARMCI_Fence here since
             * consectutive operations targeting the same process are ordered */
	    (void)ARMCI_GetS((double*)b[proc] + idx2, strideB, (double*)c + idx3, strideA,  count, ndim-1, proc);
            
            compare_patches(0., ndim, (double*)a+idx1, loA, hiA, dimsA, (double*)c+idx3, loC, hiC, dimsA);

    
        }

        free(c);
        destroy_array(b);
        free(a);
}

int nloA[MAXDIMS+1][MAXDIMS], nhiA[MAXDIMS+1][MAXDIMS];
int nloB[MAXDIMS+1][MAXDIMS], nhiB[MAXDIMS+1][MAXDIMS];
int nloC[MAXDIMS+1][MAXDIMS], nhiC[MAXDIMS+1][MAXDIMS];

int get_next_RRproc(int initialize,int ndim){
static int distance;
int proc;
    if(initialize){
       distance=nproc/2;
       if((nproc%2)!=0)distance++;
       if(nproc==1)distance=0;
       return(0);
    }
    /*send it to a different process everytime*/
    proc=(me<=((nproc%2==0)?((nproc/2)-1):(nproc/2)))?(me+distance):(me-distance); 
    if((nproc%2)!=0 && me==(nproc/2))proc=me;
    if(distance!=0){
       if(me<(nproc/2)){
         distance++;
         if((me+distance)>=nproc){
           distance=nproc/2;
           if((nproc%2)!=0)distance++; 
           distance-=me;
         }
       }
       else {
         distance--;
         if((me-distance)>=(nproc/2)){
           distance=nproc/2;
           if((nproc%2)!=0)distance++; 
           distance=distance+(me-distance);
         }
       }    
       if(ndim!=1 && MAXDIMS>nproc && (ndim%(nproc/2)==0)){
         distance=nproc/2;
         if((nproc%2)!=0)distance++;
       }
    }
    return(proc);
}

void test_nbdim()
{
int elems=1,elems1=1;
int i,j, proc,ndim,rc;
void *b[MAXDIMS+1][MAXPROC];
void *a[MAXDIMS+1], *c[MAXDIMS+1];
armci_hdl_t hdl_put[MAXDIMS+1],hdl_get[MAXDIMS+1];
int idx1=0, idx2=0, idx3=0;
    /* create shared and local arrays */
    for(ndim=1;ndim<=MAXDIMS;ndim++){
       elems1*= dimsB[ndim-1];
       elems *= dimsA[ndim-1];
       rc = ARMCI_Malloc(b[ndim], sizeof(double)*elems1);
       assert(rc==0);assert(b[ndim][me]);
       a[ndim] = malloc(sizeof(double)*elems);
       assert(a[ndim]);
       c[ndim] = malloc(sizeof(double)*elems);
       assert(c[ndim]);
       init(a[ndim], ndim, elems, dimsA);
       ARMCI_INIT_HANDLE(hdl_put+ndim);
       ARMCI_INIT_HANDLE(hdl_get+ndim);
    }
    ARMCI_AllFence();
    MP_BARRIER();

    (void)get_next_RRproc(1,0);
    for(ndim=1;ndim<=MAXDIMS;ndim++){
       strideA[0]=sizeof(double);
       strideB[0]=sizeof(double);
       for(i=0;i<ndim;i++){
         strideA[i] *= dimsA[i];
         strideB[i] *= dimsB[i];
         if(i<ndim-1){
           strideA[i+1] = strideA[i];
           strideB[i+1] = strideB[i];
         }
       }
       proc=get_next_RRproc(0,ndim);
       get_range(ndim, dimsA, nloA[ndim], nhiA[ndim]);
       new_range(ndim, dimsB, nloA[ndim], nhiA[ndim], nloB[ndim], 
                 nhiB[ndim]);
       new_range(ndim, dimsA, nloA[ndim], nhiA[ndim], nloC[ndim], 
                 nhiC[ndim]);
       if(me==0){
         print_range("local",ndim,nloA[ndim], nhiA[ndim],"-> ");
         print_range("remote",ndim,nloB[ndim], nhiB[ndim],"-> ");
         print_range("local",ndim,nloC[ndim], nhiC[ndim],"\n");
         fflush(stdout); sleep(1);
       }

       idx1 = Index(ndim, nloA[ndim], dimsA);
       idx2 = Index(ndim, nloB[ndim], dimsB);
       idx3 = Index(ndim, nloC[ndim], dimsA);
       for(j=0;j<ndim;j++)count[j]=nhiA[ndim][j]-nloA[ndim][j]+1;
       count[0]   *= sizeof(double); 

       if(ndim==1){
         (void)ARMCI_NbPut((double*)a[ndim]+idx1,(double*)b[ndim][proc]+idx2,
                           count[0], proc, (hdl_put+ndim));
       }else{
         (void)ARMCI_NbPutS((double*)a[ndim]+idx1,strideA,
                          (double*)b[ndim][proc]+idx2,
                          strideB, count, ndim-1, proc,(hdl_put+ndim));
       }
    }
sleep(5);
    MP_BARRIER();
    /*before we do gets, we have to make sure puts are complete 
      on the remote processor*/
    for(ndim=1;ndim<=MAXDIMS;ndim++)
       ARMCI_Wait(hdl_put+ndim); 
    MP_BARRIER();
    ARMCI_AllFence();

    (void)get_next_RRproc(1,0);
    
    for(ndim=1;ndim<=MAXDIMS;ndim++){
       strideA[0]=sizeof(double);
       strideB[0]=sizeof(double);
       for(i=0;i<ndim;i++){
         strideA[i] *= dimsA[i];
         strideB[i] *= dimsB[i];
         if(i<ndim-1){
           strideA[i+1] = strideA[i];
           strideB[i+1] = strideB[i];
         }
       }
       /*send it to a different process everytime*/
       proc=get_next_RRproc(0,ndim);

       idx1 = Index(ndim, nloA[ndim], dimsA);
       idx2 = Index(ndim, nloB[ndim], dimsB);
       idx3 = Index(ndim, nloC[ndim], dimsA);
       for(j=0;j<ndim;j++)count[j]=nhiA[ndim][j]-nloA[ndim][j]+1;
       count[0]   *= sizeof(double); 
       if(ndim==1){
         (void)ARMCI_NbGet((double*)b[ndim][proc]+idx2,(double*)c[ndim]+idx3,
                           count[0], proc, (hdl_get+ndim));
       }else{
         (void)ARMCI_NbGetS((double*)b[ndim][proc]+idx2,strideB,
                          (double*)c[ndim]+idx3,
                          strideA, count, ndim-1, proc,(hdl_get+ndim));
       }
    }
       
    MP_BARRIER();
    if(me==0){
       printf("Now waiting for all non-blocking calls and verifying data...\n");
       fflush(stdout);
    }
    for(ndim=1;ndim<=MAXDIMS;ndim++){
       ARMCI_Wait(hdl_get+ndim); 
       idx1 = Index(ndim, nloA[ndim], dimsA);
       idx2 = Index(ndim, nloB[ndim], dimsB);
       idx3 = Index(ndim, nloC[ndim], dimsA);
       compare_patches(0.,ndim,(double*)a[ndim]+idx1,nloA[ndim],nhiA[ndim],
                     dimsA,(double*)c[ndim]+idx3,nloC[ndim],nhiC[ndim],dimsA);
    }
    if(me==0){
       printf("OK\n");
       fflush(stdout);
    }
    
    for(ndim=1;ndim<=MAXDIMS;ndim++){
       destroy_array(b[ndim]);
       free(c[ndim]);
       free(a[ndim]);
    }
}

#define PTR_ARR_LEN 10
#define VLOOP 50
#define VEC_ELE_LEN 20  /*number of doubles in each dimention*/
#define GIOV_ARR_LEN 9

void verify_vector_data(double *data,int procs,int isput,int datalen)
{
double facto=2.89;
int i,j=0,k=0,kc=0,dst=0;
    if(isput)facto=1.89;
    for(i=0;i<datalen;i++){
       if(dst!=me)
         if(ABS((data[i] -(me+facto+dst)*((kc+1)*(j%PTR_ARR_LEN + 1))))>0.001){
           printf("\n%d:while verifying data of a op from proc=%d ",me,dst);
           printf("giov index=%d ptr_arr_index=%d \n :element index=%d",kc,
                   (j%PTR_ARR_LEN),k);
           printf(" elem was supposed to be %f but is %f",
                  (me+facto+dst)*((kc+1)*(j%PTR_ARR_LEN + 1)) ,data[i]);
           fflush(stdout);
           sleep(1);
           ARMCI_Error("vector non-blocking failed",0);
         }  
       k++;
       if(k==VEC_ELE_LEN){
         j++;k=0;
         if(j%PTR_ARR_LEN==0){
           kc++;
           if((kc%GIOV_ARR_LEN)==0){kc=0;dst++;}
         }
       }
    }
}

void test_vec_small()
{
double *getdst;
double **putsrc;
armci_giov_t dsc[MAXPROC*GIOV_ARR_LEN];
void **psrc; /*arrays of pointers to be used by giov_t*/
void **pdst; 
void *getsrc[MAXPROC]; /*to allocate mem via armci_malloc*/
void *putdst[MAXPROC]; /*to allocate mem via armci_malloc*/
armci_hdl_t hdl_put[MAXPROC],hdl_get[MAXPROC];
int i=0,j=0,k=0,kc=0,kcold=0,rc,dstproc,dst=0;
int lenpergiov;

    lenpergiov = PTR_ARR_LEN*VEC_ELE_LEN;
    rc = ARMCI_Malloc(getsrc,sizeof(double)*nproc*GIOV_ARR_LEN*lenpergiov);
    assert(rc==0);assert(getsrc[me]);
    rc = ARMCI_Malloc(putdst,sizeof(double)*nproc*GIOV_ARR_LEN*lenpergiov);
    assert(rc==0);assert(putdst[me]);

    /*first malloc for getdst and putsrc, both are 2d arrays*/
    getdst = (double *)malloc(sizeof(double)*nproc*GIOV_ARR_LEN*lenpergiov);
    putsrc = (double **)malloc(sizeof(double *)*nproc*GIOV_ARR_LEN*PTR_ARR_LEN);
    assert(getdst);assert(putsrc);
    for(i=0;i<nproc*GIOV_ARR_LEN*PTR_ARR_LEN;i++){
       putsrc[i]=(double *)malloc(sizeof(double)*VEC_ELE_LEN);
       assert(putsrc[i]);
    }
    /*allocating memory for psrc and pdst*/
    psrc = (void **)malloc(sizeof(void *)*PTR_ARR_LEN * nproc*GIOV_ARR_LEN);
    pdst = (void **)malloc(sizeof(void *)*PTR_ARR_LEN * nproc*GIOV_ARR_LEN);
    assert(pdst);assert(psrc);

    for(i=0;i<nproc*lenpergiov*GIOV_ARR_LEN;i++){
       putsrc[j][k] =(me+1.89+dst)*((kc+1)*((j%PTR_ARR_LEN) + 1));
       ((double *)getsrc[me])[i]=(me+2.89+dst)*((kc+1)*(j%PTR_ARR_LEN + 1));
       k++;
       if(k==VEC_ELE_LEN){
         j++;k=0;
         if((j%PTR_ARR_LEN)==0){
           kc++;
           if((kc%GIOV_ARR_LEN)==0){kc=0;dst++;}
         }
       }
    }
    /*********************Testing NbPutV*********************************/
    i=0;j=0;k=0;kc=0; 
    dstproc = me;
    for(i=0;i<nproc-1;i++){
       dstproc++;if(dstproc==nproc)dstproc=0;
       for(j=0; j <GIOV_ARR_LEN; j++){
         kcold=kc;
         for(k=0;k<PTR_ARR_LEN;k++,kc++){
           double *ptr;
           psrc[kc]=(void *)putsrc[PTR_ARR_LEN*(dstproc*GIOV_ARR_LEN+j)+k];
           ptr = (double *)putdst[dstproc]; 
           pdst[kc]=(void *)(ptr+lenpergiov*(GIOV_ARR_LEN*me+j)+k*VEC_ELE_LEN);
         }
         dsc[j].bytes = VEC_ELE_LEN*sizeof(double);
         dsc[j].src_ptr_array = &psrc[kcold];
         dsc[j].dst_ptr_array = &pdst[kcold];
         dsc[j].ptr_array_len = PTR_ARR_LEN;
       }
       ARMCI_INIT_HANDLE(hdl_put+dstproc);
       if((rc=ARMCI_NbPutV(dsc,GIOV_ARR_LEN,dstproc,hdl_put+dstproc)))
         ARMCI_Error("putv failed",rc);
    }
    if(me==0){
       printf("\n\tNow veryfying the vector put data for correctness");
    }
    for(i=0;i<nproc;i++)if(i!=me)ARMCI_Wait(hdl_put+i);
    sleep(1);
    MP_BARRIER();
    ARMCI_AllFence();/*every one syncs after put */
    verify_vector_data((double *)putdst[me],nproc,1,nproc*GIOV_ARR_LEN*lenpergiov);
    if(me==0){
       printf("\n\tPuts OK\n");
    }
    /****************Done Testing NbPutV*********************************/

    /*********************Testing NbGetV*********************************/
    i=0;j=0;k=0;kc=0; 
    dstproc = me;
    for(i=0;i<nproc-1;i++){
       dstproc++;if(dstproc==nproc)dstproc=0;
       for(j=0; j <GIOV_ARR_LEN; j++){
         kcold=kc;
         for(k=0;k<PTR_ARR_LEN;k++,kc++){
           double *ptr;
           ptr = getdst;
           pdst[kc]=(void *)(ptr+lenpergiov*(dstproc*GIOV_ARR_LEN+j)+k*VEC_ELE_LEN);
           ptr = (double *)(getsrc[dstproc]); 
           psrc[kc]=(void *)(ptr+lenpergiov*(me*GIOV_ARR_LEN+j)+k*VEC_ELE_LEN);
         }
         dsc[j].bytes = VEC_ELE_LEN*sizeof(double);
         dsc[j].src_ptr_array = &psrc[kcold];
         dsc[j].dst_ptr_array = &pdst[kcold];
         dsc[j].ptr_array_len = PTR_ARR_LEN;
       }
       ARMCI_INIT_HANDLE(hdl_get+dstproc);
       if((rc=ARMCI_NbGetV(dsc,GIOV_ARR_LEN,dstproc,hdl_get+dstproc)))
         ARMCI_Error("putv failed",rc);
    }
    if(me==0){
       printf("\n\tNow veryfying the vector get data for correctness");
    }
    for(i=0;i<nproc;i++)if(i!=me)ARMCI_Wait(hdl_get+i);
    sleep(1);
    MP_BARRIER();
    verify_vector_data((double *)getdst,nproc,0,nproc*GIOV_ARR_LEN*lenpergiov);
    if(me==0){
       printf("\n\tGets OK\n");
    }
    /****************Done Testing NbGetV*********************************/
}



void GetPermutedProcList(int* ProcList)
{
    int i, iswap, temp;

    if(nproc > MAXPROC) ARMCI_Error("permute_proc: nproc to big ", nproc);

    /* initialize list */
    for(i=0; i< nproc; i++) ProcList[i]=i;
    if(nproc ==1) return;

    /* every process generates different random sequence */
    (void)srand((unsigned)me);

    /* list permutation generated by random swapping */
    for(i=0; i< nproc; i++){
      iswap = (int)(rand() % nproc);
      temp = ProcList[iswap];
      ProcList[iswap] = ProcList[i];
      ProcList[i] = temp;
    }
}



/*\ Atomic Accumulate test:  remote += alpha*local
 *  Every process/or has its patch of array b updated TIMES*NPROC times.
 *  The sequence of updates is random: everybody uses a randomly permuted list
 *  and accumulate is non-collective (of-course)
\*/
void test_acc(int ndim)
{
	int dim,elems;
	int i, proc;
        void *b[MAXPROC];
        void *a, *c;
        double alpha=0.1, scale;
	int idx1, idx2;
        int *proclist = (int*)work;

        elems = 1;   
        strideA[0]=sizeof(double); 
        strideB[0]=sizeof(double);
	for(i=0;i<ndim;i++){
		strideA[i] *= dimsA[i];
		strideB[i] *= dimsB[i];
                if(i<ndim-1){
                     strideA[i+1] = strideA[i];
                     strideB[i+1] = strideB[i];
                }
		elems *= dimsA[i];

                /* set up patch coordinates: same on every processor */
                loA[i]=0;
                hiA[i]=loA[i]+1;
                loB[i]=dimsB[i]-2;
                hiB[i]=loB[i]+1;
                count[i]=hiA[i]-loA[i]+1;
	}

        /* create shared and local arrays */
        create_array(b, sizeof(double),ndim,dimsB);
        a = malloc(sizeof(double)*elems);
        assert(a);
        c = malloc(sizeof(double)*elems);
        assert(c);

	init(a, ndim, elems, dimsA);
	
	if(me==0){
            printf("--------array[%d",dimsA[0]);
	    for(dim=1;dim<ndim;dim++)printf(",%d",dimsA[dim]);
	    printf("]--------\n");
        }

        GetPermutedProcList(proclist);

 	idx1 = Index(ndim, loA, dimsA);
	idx2 = Index(ndim, loB, dimsB);
	count[0]   *= sizeof(double); /* convert range to bytes at stride level zero */
        
        /* initialize all elements of array b to zero */
	elems = 1;
        for(i=0;i<ndim;i++)elems *= dimsB[i];
        for(i=0;i<elems;i++)((double*)b[me])[i]=0.;

        sleep(1);

        if(me==0){
               print_range("patch",ndim,loA, hiA," -> ");
               print_range("patch",ndim,loB, hiB,"\n");
               fflush(stdout);
        }

        ARMCI_AllFence();
        MP_BARRIER();
        for(i=0;i<TIMES*nproc;i++){ 
            proc=proclist[i%nproc];
            (void)ARMCI_AccS(ARMCI_ACC_DBL,&alpha,(double*)a + idx1, strideA, 
                   (double*)b[proc] + idx2, strideB, count, ndim-1, proc);
        }

/*	sleep(9);*/
        ARMCI_AllFence();
        MP_BARRIER();

        /* copy my patch into local array c */
	(void)ARMCI_GetS((double*)b[me] + idx2, strideB, (double*)c + idx1, strideA,  count, ndim-1, me);

        scale = alpha*TIMES*nproc; 

        scale_patch(scale, ndim, (double*)a+idx1, loA, hiA, dimsA);
        
        compare_patches(.0001, ndim, (double*)a+idx1, loA, hiA, dimsA, (double*)c+idx1, loA, hiA, dimsA);
        MP_BARRIER();

        if(0==me){
            printf(" OK\n\n");
            fflush(stdout);
        }

        free(c);
        destroy_array(b);
        free(a);
}


/*************************** vector interface *********************************\
 * tests vector interface for transfers of triangular sections of a 2-D array *
 ******************************************************************************/
void test_vector()
{
	int dim,elems,ndim,cols,rows,mrc;
	int i, proc, loop;
        int rc;
        int idx1, idx3;
        void *b[MAXPROC];
        void *a, *c;
        armci_giov_t dsc[MAX_DIM_VAL];
        void *psrc[MAX_DIM_VAL];
        void *pdst[MAX_DIM_VAL];

	elems = 1;   
        ndim  = 2;
	for(i=0;i<ndim;i++){
                dimsA[i]=MAX_DIM_VAL;
                dimsB[i]=MAX_DIM_VAL+1;
		elems *= dimsA[i];
	}

        /* create shared and local arrays */
        create_array(b, sizeof(double),ndim,dimsB);
        a = malloc(sizeof(double)*elems);
        assert(a);
        c = malloc(sizeof(double)*elems);
        assert(c);

	init(a, ndim, elems, dimsA);
	
	if(me==0){
            printf("--------array[%d",dimsA[0]);
	    for(dim=1;dim<ndim;dim++)printf(",%d",dimsA[dim]);
	    printf("]--------\n");
        }
        sleep(1);

	for(loop=0;loop<LOOP;loop++){
	    get_range(ndim, dimsA, loA, hiA);
	    new_range(ndim, dimsB, loA, hiA, loB, hiB);
	    new_range(ndim, dimsA, loA, hiA, loC, hiC);

            proc=nproc-1-me;

            if(me==0){
	       print_range("local",ndim,loA, hiA,"-> ");
	       print_range("remote",ndim,loB, hiB,"-> ");
	       print_range("local",ndim,loC, hiC,"\n");
            }

/*            printf("array at source\n");*/
/*            print_2D_double((double *)a, dimsA[0], loA, hiA);*/

            cols =  hiA[1]-loA[1]+1; 
            rows =  hiA[0]-loA[0]+1; 
            mrc =MIN(cols,rows);

            /* generate a data descriptor for a lower-triangular patch */
            for(i=0; i < mrc; i++){
               int ij[2];
               int idx;

               ij[0] = loA[0]+i;
               ij[1] = loA[1]+i;
               idx = Index(ndim, ij, dimsA);
               psrc[i]= (double*)a + idx;

               ij[0] = loB[0]+i;
               ij[1] = loB[1]+i;
               idx = Index(ndim, ij, dimsB);
               pdst[i]= (double*)b[proc] + idx;

               dsc[i].bytes = (rows-i)*sizeof(double);
               dsc[i].src_ptr_array = &psrc[i];
               dsc[i].dst_ptr_array = &pdst[i];

               /* assume each element different in size (not true in rectangular patches) */ 
               dsc[i].ptr_array_len = 1; 
            }

            if((rc=ARMCI_PutV(dsc, mrc, proc)))ARMCI_Error("putv failed ",rc);

/*            printf("array at destination\n");*/
/*            print_2D_double((double *)b[proc], dimsB[0], loB, hiB);*/

            /* generate a data descriptor for the upper-triangular patch */
            /* there is one less element since diagonal is excluded      */
            for(i=1; i < cols; i++){
               int ij[2];

               ij[0] = loA[0];
               ij[1] = loA[1]+i;
               psrc[i-1]= (double*)a + Index(ndim, ij, dimsA);

               ij[0] = loB[0];
               ij[1] = loB[1]+i;
               pdst[i-1]= (double*)b[proc] + Index(ndim, ij, dimsB);

               mrc = MIN(i,rows);
               dsc[i-1].bytes = mrc*sizeof(double);
               dsc[i-1].src_ptr_array = &psrc[i-1];
               dsc[i-1].dst_ptr_array = &pdst[i-1];

               /* assume each element different in size (not true in rectangular patches) */ 
               dsc[i-1].ptr_array_len = 1; 
            }

            if((cols-1))if((rc=ARMCI_PutV(dsc, cols-1, proc)))
                               ARMCI_Error("putv(2) failed ",rc);

            /* we get back entire rectangular patch */
            for(i=0; i < cols; i++){
               int ij[2];
               ij[0] = loB[0];
               ij[1] = loB[1]+i;
               psrc[i]= (double*)b[proc] + Index(ndim, ij, dimsB);

               ij[0] = loC[0];
               ij[1] = loC[1]+i;
               pdst[i]= (double*)c + Index(ndim, ij, dimsA);
            }

            dsc[0].bytes = rows*sizeof(double);
            dsc[0].src_ptr_array = psrc;
            dsc[0].dst_ptr_array = pdst;
            dsc[0].ptr_array_len = cols; 

            /* note that we do not need ARMCI_Fence here since
             * consecutive operations targeting the same process are ordered */
            if((rc=ARMCI_GetV(dsc, 1, proc)))ARMCI_Error("getv failed ",rc);
            
	    idx1 = Index(ndim, loA, dimsA);
	    idx3 = Index(ndim, loC, dimsA);
            compare_patches(0., ndim, (double*)a+idx1, loA, hiA, dimsA, (double*)c+idx3, loC, hiC, dimsA);
    
        }

        free(c);
        destroy_array(b);
        free(a);
}


/*\ Atomic Accumulate test for vector API:  remote += alpha*local
 *  Every process/or has its patch of array b updated TIMES*NPROC times.
 *  The sequence of updates is random: everybody uses a randomly permuted list
 *  and accumulate is non-collective (of-course)
\*/
void test_vector_acc()
{
	int dim,elems,bytes;
	int i, j, proc, rc, one=1;
        void *b[MAXPROC];
        void *psrc[ELEMS/2], *pdst[ELEMS/2];
        void *a, *c;
        double alpha=0.1, scale;
        int *proclist = (int*)work;
        armci_giov_t dsc;

        elems = ELEMS;
        dim =1;
        bytes = sizeof(double)*elems;

        /* create shared and local arrays */
        create_array(b, sizeof(double),dim,&elems);
        a = malloc(bytes);
        assert(a);
        c = malloc(bytes);
        assert(c);

	init(a, dim, elems, &elems);
	
	if(me==0){
            printf("--------array[%d",elems);
	    printf("]--------\n");
            fflush(stdout);
        }

        GetPermutedProcList(proclist);
        
        /* initialize all elements of array b to zero */
        for(i=0;i<elems;i++)((double*)b[me])[i]=0.;

        sleep(1);

        dsc.bytes = sizeof(double);
        dsc.src_ptr_array = psrc;
        dsc.dst_ptr_array = pdst;
        dsc.ptr_array_len = elems/2; 


        MP_BARRIER();
        for(i=0;i<TIMES*nproc;i++){ 

/*            proc=proclist[i%nproc];*/
            proc=0;

            /* accumulate even numbered elements */
            for(j=0; j<elems/2; j++){
                psrc[j]= 2*j + (double*)a;
                pdst[j]= 2*j + (double*)b[proc];
            }
            if((rc = ARMCI_AccV(ARMCI_ACC_DBL, &alpha, &dsc, 1, proc)))
                     ARMCI_Error("accumlate failed",rc);
/*            for(j=0; j<elems; j++)
                printf("%d %lf %lf\n",j, *(j+ (double*)b[proc]), *(j+ (double*)a));
*/
            /* accumulate odd numbered elements */
            for(j=0; j< elems/2; j++){
                psrc[j]= 2*j+1 + (double*)a;
                pdst[j]= 2*j+1 + (double*)b[proc];
            }
            (void)ARMCI_AccV(ARMCI_ACC_DBL, &alpha, &dsc, 1, proc);

/*            for(j=0; j<elems; j++)
                printf("%d %lf %lf\n",j, *(j+ (double*)a), *(j+ (double*)b[proc]));
*/
        }

        ARMCI_AllFence();
        MP_BARRIER();

        /* copy my patch into local array c */
	assert(!ARMCI_Get((double*)b[proc], c, bytes, proc));

/*        scale = alpha*TIMES*nproc; */
        scale = alpha*TIMES*nproc*nproc; 
        scale_patch(scale, dim, a, &one, &elems, &elems);
        
        compare_patches(.0001, dim, a, &one, &elems, &elems, c, &one, &elems, &elems);
        MP_BARRIER();

        if(0==me){
            printf(" OK\n\n");
            fflush(stdout);
        }

        free(c);
        destroy_array((void**)b);
        free(a);
}



void test_fetch_add()
{
    int rc, bytes, i, val, times =0;
    int *arr[MAXPROC];

    /* shared variable is located on processor 0 */
    bytes = me == 0 ? sizeof(int) : 0;

    rc = ARMCI_Malloc((void**)arr,bytes);
    assert(rc==0);
    MP_BARRIER();

    if(me == 0) *arr[0] = 0;  /* initialization */

    MP_BARRIER();

    /* show what everybody gets */
    rc = ARMCI_Rmw(ARMCI_FETCH_AND_ADD, &val, arr[0], 1, 0);
    assert(rc==0);

    for(i = 0; i< nproc; i++){
        if(i==me){
            printf("process %d got value of %d\n",i,val);
            fflush(stdout);
        }
        MP_BARRIER();
    }

    if(me == 0){
      printf("\nIncrement the shared counter until reaches %d\n",LOOP);
      fflush(stdout);
    }
    
    MP_BARRIER();

    /* now increment the counter value until reaches LOOP */
    while(val<LOOP){
          rc = ARMCI_Rmw(ARMCI_FETCH_AND_ADD, &val, arr[0], 1, 0);
          assert(rc==0);
          times++;
    }

    for(i = 0; i< nproc; i++){
        if(i==me){
            printf("process %d incremented the counter %d times value=%d\n",i,times,val);
            fflush(stdout);
        }
        MP_BARRIER();
    }


    if(me == 0) *arr[0] = 0;  /* set it back to 0 */
    if(me == 0){
       printf("\nNow everybody increments the counter %d times\n",LOOP); 
       fflush(stdout);
    }

    ARMCI_AllFence();
    MP_BARRIER();

    for(i = 0; i< LOOP; i++){
          rc = ARMCI_Rmw(ARMCI_FETCH_AND_ADD, &val, arr[0], 1, 0);
          assert(rc==0);
    }

    ARMCI_AllFence();
    MP_BARRIER();

    if(me == 0){
       printf("The final value is %d, should be %d.\n\n",*arr[0],LOOP*nproc); 
       fflush(stdout);
       if( *arr[0] != LOOP*nproc) ARMCI_Error("failed ...",*arr[0]);
    }

    ARMCI_Free(arr[me]);
}


#define LOCKED -1
void test_swap()
{
    int rc, bytes, i, val, whatever=-8999;
    int *arr[MAXPROC];

    /* shared variable is located on processor 0 */
    bytes = me == 0 ? sizeof(int) : 0;

    rc = ARMCI_Malloc((void**)arr,bytes);
    assert(rc==0);
    MP_BARRIER();

    if(me == 0) *arr[0] = 0;  /* initialization */

    ARMCI_AllFence();
    MP_BARRIER();
    for(i = 0; i< LOOP; i++){
          val = LOCKED;
          do{
            rc = ARMCI_Rmw(ARMCI_SWAP, &val, arr[0], whatever, 0);
            assert(rc==0);
          }while (val == LOCKED); 
          val++;
          rc = ARMCI_Rmw(ARMCI_SWAP, &val, arr[0], whatever, 0);
          assert(rc==0);
    }


    ARMCI_AllFence();
    MP_BARRIER();

    if(me == 0){
       printf("The final value is %d, should be %d.\n\n",*arr[0],LOOP*nproc); 
       fflush(stdout);
       if( *arr[0] != LOOP*nproc) ARMCI_Error("failed ...",*arr[0]);
    }

    ARMCI_Free(arr[me]);
}




void test_memlock()
{
        int dim,elems,bytes;
        int i, j,k, proc;
        double *b[MAXPROC];
        double *a, *c;
#if 0
        int *proclist = (int*)work;
#endif
                void *pstart, *pend;
                int first, last;
                void armci_lockmem(void*, void*, int);
                void armci_unlockmem(void);

        elems = ELEMS;
        dim =1;

                bytes = elems*sizeof(double);

        /* create shared and local arrays */
        create_array((void**)b, sizeof(double),dim,&elems);
        a = (double*)malloc(bytes);
        assert(a);
        c = (double*)malloc(bytes);
        assert(c);
        
        /* initialize all elements of array b to zero */
        for(i=0;i<elems;i++)b[me][i]=-1.;

        sleep(1);

        proc=0;
                for(i=0;i<ELEMS/5;i++)a[i]=me;

        MP_BARRIER();
        for(j=0;j<10*TIMES;j++){ 
         for(i=0;i<TIMES*nproc;i++){ 
            first = rand()%(ELEMS/2);
                    last = first+ELEMS/5 -1;
                        pstart = b[proc]+first;
                        pend = b[proc]+last+1;
                        elems = last -first +1;
                        bytes = sizeof(double)*elems;

            armci_lockmem(pstart,pend,proc);
            assert(!ARMCI_Put(a, pstart, bytes, proc));
            assert(!ARMCI_Get(pstart, c, bytes, proc));
            assert(!ARMCI_Get(pstart, c, bytes, proc));
            armci_unlockmem();
            for(k=0;k<elems;k++)if(a[k]!=c[k]){
                printf("%d: error patch (%d:%d) elem=%d val=%f\n",me,first,last,k,c[k]);
                fflush(stdout);
                ARMCI_Error("failed is ",(int)c[k]);
            }

          }
          if(0==me)fprintf(stderr,"done %d\n",j);
                }

        MP_BARRIER();


        if(0==me){
            printf(" OK\n\n");
            fflush(stdout);
        }

        free(c);
        destroy_array((void**)b);
        free(a);
}

void test_rput()
{
    int i, elems = nproc, one=1;
    int *idst[MAXPROC], idst_get[MAXPROC], *isrc_get[MAXPROC];
    long *ldst[MAXPROC], ldst_get[MAXPROC], *lsrc_get[MAXPROC];
    float *fdst[MAXPROC], fdst_get[MAXPROC], *fsrc_get[MAXPROC];
    double *ddst[MAXPROC], ddst_get[MAXPROC], *dsrc_get[MAXPROC];

    create_array((void**)idst, sizeof(int),1, &elems);
    create_array((void**)ldst, sizeof(long),1, &elems);
    create_array((void**)fdst, sizeof(float),1, &elems);
    create_array((void**)ddst, sizeof(double),1, &elems);     
    create_array((void**)isrc_get, sizeof(int),1, &one);
    create_array((void**)lsrc_get, sizeof(long),1, &one);
    create_array((void**)fsrc_get, sizeof(float),1, &one);
    create_array((void**)dsrc_get, sizeof(double),1, &one); 
    
    for(i=0; i<elems; i++) {
      idst[me][i]=0; ldst[me][i]=0; fdst[me][i]=0.0; ddst[me][i]=0.0;
      idst_get[i]=0; ldst_get[i]=0; fdst_get[i]=0.0; ddst_get[i]=0.0;      
    }
    isrc_get[me][0]=100*(me+1); lsrc_get[me][0]=100*(me+1);
    fsrc_get[me][0]=100.01*(me+1); dsrc_get[me][0]=100.001*(me+1);

    
    ARMCI_AllFence();
    MP_BARRIER();
    for(i=0; i<nproc; i++) {
      ARMCI_PutValueInt(10*(me+1), (void *)&idst[i][me], i);
      ARMCI_PutValueLong((long)10*(me+1), (void *)&ldst[i][me], i);
      ARMCI_PutValueFloat(10.01*(me+1), (void *)&fdst[i][me], i);
      ARMCI_PutValueDouble(10.001*(me+1), (void *)&ddst[i][me], i);
    }

    for(i=0; i<nproc; i++) {
      idst_get[i] = ARMCI_GetValueInt(isrc_get[i], i);
      ldst_get[i] = ARMCI_GetValueLong(lsrc_get[i], i);
      fdst_get[i] = ARMCI_GetValueFloat(fsrc_get[i], i);
      ddst_get[i] = ARMCI_GetValueDouble(dsrc_get[i], i);
    }
    
    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0)printf("int data type: ");
    for(i=0; i<elems; i++) {
      if(idst[me][i]!=10*(i+1))
	ARMCI_Error("Integer register-originated put failed", 0);
      if(idst_get[i]!=100*(i+1))
	ARMCI_Error("Integer register-originated get failed", 0);
    }
    
    if(me==0)printf("OK\nlong data type: ");
    for(i=0; i<elems; i++) {
      if(ldst[me][i]!=10*(i+1))
	ARMCI_Error("Long register-originated put failed", 0);
      if(ldst_get[i]!=100*(i+1))
	ARMCI_Error("Long register-originated get failed", 0);
    }
    if(me==0)printf("OK\nfloat data type: ");
    for(i=0; i<elems; i++) {
      if( ABS(fdst[me][i]-10.01*(i+1)) > 0.1)
	ARMCI_Error("Float register-originated put failed", 0);
      if( ABS(fdst_get[i]-100.01*(i+1)) > 0.1)
	ARMCI_Error("Float register-originated get failed", 0);
    }
    if(me==0)printf("OK\ndouble data type: ");
    for(i=0; i<elems; i++) {
      if(ABS(ddst[me][i]-10.001*(i+1)) > 0.1)
	ARMCI_Error("Double register-originated put failed",0);
      if(ABS(ddst_get[i]-100.001*(i+1)) > 0.1)
	ARMCI_Error("Double register-originated get failed",0);
    }
    if(me==0){printf("OK\n"); fflush(stdout);}
    
    ARMCI_AllFence();
    MP_BARRIER();
    
    
    destroy_array((void **)idst);
    destroy_array((void **)ldst);
    destroy_array((void **)fdst);
    destroy_array((void **)ddst);

    destroy_array((void **)isrc_get);
    destroy_array((void **)lsrc_get);
    destroy_array((void **)fsrc_get);
    destroy_array((void **)dsrc_get);
}


#define MAXELEMS      6400
#define NUMAGG        20   /* NUMAGG < MAXELEMS/10 */
#define MAX_REQUESTS  325 /* MAXELEMS/NUMAGG */
#define DUMMY         50

void test_aggregate() {
  
    int i, j, k, rc, bytes, elems[2] = {MAXPROC, MAXELEMS};
    double *ddst_put[MAXPROC];
    double *ddst_get[MAXPROC];
    double *dsrc[MAXPROC];
    armci_hdl_t usr_hdl_put[MAXPROC];
    armci_hdl_t usr_hdl_get[MAXPROC];
    armci_giov_t darr;
    void *src_ptr[MAX_REQUESTS], *dst_ptr[MAX_REQUESTS];
    int start = 0, end = 0;
        
    create_array((void**)ddst_put, sizeof(double),2, elems);
    create_array((void**)ddst_get, sizeof(double),2, elems);
    create_array((void**)dsrc, sizeof(double),1, &elems[1]);
    
    for(i=0; i<elems[1]; i++) dsrc[me][i]=i*1.001*(me+1);
    for(i=0; i<elems[0]*elems[1]; i++) {
      ddst_put[me][i]=0.0;
      ddst_get[me][i]=0.0;
    }
    
    ARMCI_Barrier();
    for(i=0; i<nproc; i++) ARMCI_INIT_HANDLE(&usr_hdl_put[i]);
    for(i=0; i<nproc; i++) ARMCI_INIT_HANDLE(&usr_hdl_get[i]);
    for(i=0; i<nproc; i++) ARMCI_SET_AGGREGATE_HANDLE(&usr_hdl_put[i]);
    for(i=0; i<nproc; i++) ARMCI_SET_AGGREGATE_HANDLE(&usr_hdl_get[i]);    

    /* Testing aggregate put */
    for(i=0; i<nproc; i++) {

      start = 0; end = DUMMY*NUMAGG; 
      for(j=start; j<end; j++) {  
	bytes = sizeof(double);
	ARMCI_NbPutValueDouble(dsrc[me][j], &ddst_put[i][me*elems[1]+j], i, 
			       &usr_hdl_put[i]);
      }
      
      start = end; end = start + DUMMY*NUMAGG;
      for(j=start, k=0; j<end; j+=NUMAGG, k++) {
	src_ptr[k] = (void *)&dsrc[me][j];
	dst_ptr[k] = (void *)&ddst_put[i][me*elems[1]+j];
      }
      darr.src_ptr_array = src_ptr;
      darr.dst_ptr_array = dst_ptr;
      darr.bytes = NUMAGG*sizeof(double);
      darr.ptr_array_len = k;
      if((rc=ARMCI_NbPutV(&darr, 1, i, &usr_hdl_put[i])))
	ARMCI_Error("armci_nbputv failed\n",rc);

      start = end; end = start + DUMMY*NUMAGG;
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbPutS(&dsrc[me][j], NULL, &ddst_put[i][me*elems[1]+j], NULL, 
			    &bytes, 0, i, &usr_hdl_put[i])))
	  ARMCI_Error("armci_nbputs failed\n",rc);
      }
 
      start = end; end = elems[1]; 
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbPut(&dsrc[me][j], &ddst_put[i][me*elems[1]+j], bytes,
			   i, &usr_hdl_put[i])))
	  ARMCI_Error("armci_nbput failed\n",rc);
      }

    }
    
    for(i=0; i<nproc; i++) ARMCI_Wait(&usr_hdl_put[i]);

    /* Testing aggregate get */
    for(i=0; i<nproc; i++) {
      
      start = 0; end = DUMMY*NUMAGG; 
      for(j=start, k=0; j<end; j+=NUMAGG, k++) {
	src_ptr[k] = (void *)&dsrc[i][j];
	dst_ptr[k] = (void *)&ddst_get[me][i*elems[1]+j];
      }
      darr.src_ptr_array = src_ptr;
      darr.dst_ptr_array = dst_ptr;
      darr.bytes = NUMAGG*sizeof(double);
      darr.ptr_array_len = k;
      if((rc=ARMCI_NbGetV(&darr, 1, i, &usr_hdl_get[i])))
	ARMCI_Error("armci_nbgetv failed\n", rc);

      start = end; end = start + DUMMY*NUMAGG;
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbGetS(&dsrc[i][j], NULL, &ddst_get[me][i*elems[1]+j], NULL, 
			    &bytes, 0, i, &usr_hdl_get[i])))
	  ARMCI_Error("armci_nbputs failed\n",rc);
      }

      start = end; end = elems[1];
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbGet(&dsrc[i][j], &ddst_get[me][i*elems[1]+j], bytes,
			   i, &usr_hdl_get[i])))
	  ARMCI_Error("armci_nbget failed\n",rc);
      }
    }

    for(i=0; i<nproc; i++) ARMCI_Wait(&usr_hdl_get[i]);
        
    
    MP_BARRIER();
    ARMCI_AllFence();
    MP_BARRIER();
    
    for(i=0; i<nproc; i++) {
      for(j=0; j<elems[1]; j++) {
	if( ABS(ddst_put[me][i*elems[1]+j]-j*1.001*(i+1)) > 0.1) {
	  ARMCI_Error("aggregate put failed...1", 0);
	}
      }
    }
    MP_BARRIER();
    if(me==0) printf("  aggregate put ..O.K.\n"); fflush(stdout);

    for(i=0; i<nproc; i++) {
      for(j=0; j<elems[1]; j++) {
	if( ABS(ddst_get[me][i*elems[1]+j]-j*1.001*(i+1)) > 0.1) {
	  ARMCI_Error("aggregate get failed...1", 0);
	}
      }
    }
    MP_BARRIER();
    if(me==0) printf("  aggregate get ..O.K.\n"); fflush(stdout);
    
    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0){printf("O.K.\n"); fflush(stdout);}
    destroy_array((void **)ddst_put);
    destroy_array((void **)ddst_get);
    destroy_array((void **)dsrc);
}

void test_implicit() {
  
    int i, j, k, rc, bytes, elems[2] = {MAXPROC, MAXELEMS};
    double *ddst_put[MAXPROC];
    double *ddst_get[MAXPROC];
    double *dsrc[MAXPROC];
    armci_giov_t darr;
    void *src_ptr[MAX_REQUESTS], *dst_ptr[MAX_REQUESTS];
    int start = 0, end = 0;
    armci_hdl_t usr_hdl[MAXPROC];
    
    create_array((void**)ddst_put, sizeof(double),2, elems);
    create_array((void**)ddst_get, sizeof(double),2, elems);
    create_array((void**)dsrc, sizeof(double),1, &elems[1]);
    
    for(i=0; i<elems[1]; i++) dsrc[me][i]=i*1.001*(me+1);
    for(i=0; i<elems[0]*elems[1]; i++) {
      ddst_put[me][i]=0.0;
      ddst_get[me][i]=0.0;
    }
    
    MP_BARRIER();
    for(i=0; i<nproc; i++) ARMCI_INIT_HANDLE(&usr_hdl[i]);

    for(i=0; i<nproc; i++) {
      
      start = 0; end = DUMMY*NUMAGG; 
      for(j=start; j<end; j++) {  
	bytes = sizeof(double);
	ARMCI_NbPutValueDouble(dsrc[me][j], &ddst_put[i][me*elems[1]+j], i, 
			       NULL);
      }
      
      start = end; end = start + DUMMY*NUMAGG;
      for(j=start, k=0; j<end; j+=NUMAGG, k++) {
	src_ptr[k] = (void *)&dsrc[me][j];
	dst_ptr[k] = (void *)&ddst_put[i][me*elems[1]+j];
      }
      darr.src_ptr_array = src_ptr;
      darr.dst_ptr_array = dst_ptr;
      darr.bytes = NUMAGG*sizeof(double);
      darr.ptr_array_len = k;
      if((rc=ARMCI_NbPutV(&darr, 1, i, NULL)))
	ARMCI_Error("armci_nbputv failed\n",rc);

      start = end; end = start + DUMMY*NUMAGG;
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbPutS(&dsrc[me][j], NULL, &ddst_put[i][me*elems[1]+j], NULL, 
			    &bytes, 0, i, NULL)))
	  ARMCI_Error("armci_nbputs failed\n",rc);
      }

      start = end; end = elems[1]; 
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbPut(&dsrc[me][j], &ddst_put[i][me*elems[1]+j], bytes,
			   i, NULL)))
 	  ARMCI_Error("armci_nbput failed\n",rc);
      }
    }


    for(i=0; i<nproc; i++) {

      start = 0; end = DUMMY*NUMAGG; 
      for(j=start, k=0; j<end; j+=NUMAGG, k++) {
	src_ptr[k] = (void *)&dsrc[i][j];
	dst_ptr[k] = (void *)&ddst_get[me][i*elems[1]+j];
      }
      darr.src_ptr_array = src_ptr;
      darr.dst_ptr_array = dst_ptr;
      darr.bytes = NUMAGG*sizeof(double);
      darr.ptr_array_len = k;
      if((rc=ARMCI_NbGetV(&darr, 1, i, NULL)))
	ARMCI_Error("armci_nbgetv failed\n", rc);

      start = end; end = start + DUMMY*NUMAGG;
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbGetS(&dsrc[i][j], NULL, &ddst_get[me][i*elems[1]+j], NULL, 
			    &bytes, 0, i, NULL)))
	  ARMCI_Error("armci_nbputs failed\n",rc);
      }

      start = end; end = elems[1];
      for(j=start; j<end; j+=NUMAGG) {  
	bytes = sizeof(double)*NUMAGG;
	if((rc=ARMCI_NbGet(&dsrc[i][j], &ddst_get[me][i*elems[1]+j], bytes,
			   i, NULL)))
	  ARMCI_Error("armci_nbget failed\n",rc);
      }
    }

    ARMCI_WaitAll(); 
        
    
    MP_BARRIER();
    ARMCI_AllFence();
    MP_BARRIER();
    
    for(i=0; i<nproc; i++) {
      for(j=0; j<elems[1]; j++) {
	if( ABS(ddst_put[me][i*elems[1]+j]-j*1.001*(i+1)) > 0.1) {
	  ARMCI_Error("implicit handle(s) failed...(a)", 0);
	}
      }
    }
    MP_BARRIER();

    for(i=0; i<nproc; i++) {
      for(j=0; j<elems[1]; j++) {
	if( ABS(ddst_get[me][i*elems[1]+j]-j*1.001*(i+1)) > 0.1) {
	  ARMCI_Error("implicit handles(s) failed...(b)", 0);
	}
      }
    }

    MP_BARRIER();
    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0){printf("O.K.\n\n"); fflush(stdout);}
    destroy_array((void **)ddst_put);
    destroy_array((void **)ddst_get);
    destroy_array((void **)dsrc);
}


/* we need to rename main if linking with frt compiler */
#ifdef FUJITSU_FRT
#define main MAIN__
#endif

int main(int argc, char* argv[])
{
    int ndim;

    MP_INIT(argc, argv);
    MP_PROCS(&nproc);
    MP_MYID(&me);

/*    printf("nproc = %d, me = %d\n", nproc, me);*/
    
    if(nproc>MAXPROC && me==0)
       ARMCI_Error("Test works for up to %d processors\n",MAXPROC);

    if(me==0){
       printf("ARMCI test program (%d processes)\n",nproc); 
       fflush(stdout);
       sleep(1);
    }
    
    ARMCI_Init();

/*
       if(me==1)armci_die("process 1 committing suicide",1);
*/

        if(me==0){
           printf("\nTesting strided gets and puts\n");
           printf("(Only std output for process 0 is printed)\n\n"); 
           fflush(stdout);
           sleep(1);
        }
        for(ndim=1; ndim<= MAXDIMS; ndim++) test_dim(ndim);
        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting non-blocking gets and puts\n");
           fflush(stdout);
           sleep(1);
        }
        test_nbdim(); 
        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting non-blocking vector gets and puts\n");
           fflush(stdout);
           sleep(1);
        }
        test_vec_small();
        ARMCI_AllFence();
        MP_BARRIER();


        if(me==0){
           printf("\nTesting atomic accumulate\n");
           fflush(stdout);
           sleep(1);
        }
        for(ndim=1; ndim<= MAXDIMS; ndim++) test_acc(ndim); 
        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting Vector Interface using triangular patches of a 2-D array\n\n");
           fflush(stdout);
           sleep(1);
        }

        test_vector();
        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting Accumulate with Vector Interface\n\n");
           fflush(stdout);
           sleep(1);
        }
        test_vector_acc();

        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting atomic fetch&add\n");
           printf("(Std Output for all processes is printed)\n\n"); 
           fflush(stdout);
           sleep(1);
        }
        MP_BARRIER();

        test_fetch_add();

        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting atomic swap\n");
           fflush(stdout);
        }
        test_swap();
        ARMCI_AllFence();
        MP_BARRIER();

        if(me==0){
           printf("\nTesting register-originated put and get\n");
           fflush(stdout);
           sleep(1);
        }
        test_rput(); 
        ARMCI_AllFence();
        MP_BARRIER();

	if(me==0){
	  printf("\nTesting aggregate put/get requests\n");
	  fflush(stdout);
	}
	
	/** 
	 * Aggregate put/get requests cannot be tested for\ number of procs 
	 * greater than 32. (Current implementation of aggregate put/get 
	 * can use at the maximum of 32 handles (defined by macro 
	 * _MAX_AGG_BUFFERS in aggregate.c). This test case is written in 
	 * such a way that each process puts/gets data to all the other 
	 * processes, thus the number of aggregate handle used is equal to 
	 * the number of processes created.
	*/
	if(nproc > 32) {
	  if(me==0){
	    printf("\n WARNING: Aggregate put/get requests cannot be tested for number of procs greater than 32.\n\n");
	    fflush(stdout);
	  }
	}
	else
	  test_aggregate();
	
	ARMCI_AllFence();
	MP_BARRIER();

	if(me==0){
	  printf("\nTesting implicit handles\n");
	  fflush(stdout);
	}
	test_implicit();
	
	ARMCI_AllFence();
	MP_BARRIER();


        MP_BARRIER();
        /*test_memlock();*/

        MP_BARRIER();
	if(me==0){printf("All tests passed\n"); fflush(stdout);}
    sleep(2);

    MP_BARRIER();
    ARMCI_Finalize();
    MP_FINALIZE();
    return(0);
}
