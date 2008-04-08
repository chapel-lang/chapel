#ifndef _ACC_H_
#define _ACC_H_


#ifdef NOFORT
#    define ATR  
#else

#  define ATR FATR

#if defined(AIX) || defined(NOUNDERSCORE)
#    define I_ACCUMULATE_2D	i_accumulate_2d_u
#    define D_ACCUMULATE_2D	d_accumulate_2d_u
#    define C_ACCUMULATE_2D	c_accumulate_2d_u
#    define Z_ACCUMULATE_2D	z_accumulate_2d_u
#    define F_ACCUMULATE_2D	f_accumulate_2d_u
#elif defined(BGML)
#    define I_ACCUMULATE_2D     i_accumulate_2d_u__
#    define D_ACCUMULATE_2D     d_accumulate_2d_u__
#    define C_ACCUMULATE_2D     c_accumulate_2d_u__
#    define Z_ACCUMULATE_2D     z_accumulate_2d_u__
#    define F_ACCUMULATE_2D     f_accumulate_2d_u__
#elif defined(SGI_)
#    define I_ACCUMULATE_2D     i_accumulate_2d_u_
#    define D_ACCUMULATE_2D     d_accumulate_2d_u_
#    define C_ACCUMULATE_2D     c_accumulate_2d_u_
#    define Z_ACCUMULATE_2D     z_accumulate_2d_u_
#    define F_ACCUMULATE_2D     f_accumulate_2d_u_
#elif !defined(CRAY) && !defined(WIN32) && !defined(HITACHI) ||defined(__crayx1)
#    define I_ACCUMULATE_2D     i_accumulate_2d_
#    define D_ACCUMULATE_2D     d_accumulate_2d_
#    define C_ACCUMULATE_2D     c_accumulate_2d_
#    define Z_ACCUMULATE_2D     z_accumulate_2d_
#    define F_ACCUMULATE_2D     f_accumulate_2d_
#elif defined(WIN32)
#    define L_ACCUMULATE_2D     I_ACCUMULATE_2D
#    define L_ACCUMULATE_1D     I_ACCUMULATE_1D
#endif

#if defined(CRAY) && !defined(__crayx1)
#undef D_ACCUMULATE_2D 
#define  D_ACCUMULATE_2D daxpy_2d_
#endif
#endif

#if defined(AIX) || defined(NOUNDERSCORE)
#    define I_ACCUMULATE_1D     i_accumulate_1d  
#    define D_ACCUMULATE_1D     d_accumulate_1d  
#    define C_ACCUMULATE_1D     c_accumulate_1d  
#    define Z_ACCUMULATE_1D     z_accumulate_1d  
#    define F_ACCUMULATE_1D     f_accumulate_1d  
#elif defined(BGML)
#    define I_ACCUMULATE_1D     i_accumulate_1d__
#    define D_ACCUMULATE_1D     d_accumulate_1d__
#    define C_ACCUMULATE_1D     c_accumulate_1d__
#    define Z_ACCUMULATE_1D     z_accumulate_1d__
#    define F_ACCUMULATE_1D     f_accumulate_1d__
#elif !defined(CRAY) && !defined(WIN32) && !defined(HITACHI) ||defined(__crayx1)
#    define I_ACCUMULATE_1D     i_accumulate_1d_ 
#    define D_ACCUMULATE_1D     d_accumulate_1d_ 
#    define C_ACCUMULATE_1D     c_accumulate_1d_ 
#    define Z_ACCUMULATE_1D     z_accumulate_1d_ 
#    define F_ACCUMULATE_1D     f_accumulate_1d_ 
#elif defined(WIN32)
#    define L_ACCUMULATE_2D     I_ACCUMULATE_2D
#    define L_ACCUMULATE_1D     I_ACCUMULATE_1D
#endif


void ATR I_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*); 
void ATR L_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*); 
void ATR D_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*); 
void ATR C_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*); 
void ATR Z_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*); 
void ATR I_ACCUMULATE_1D(void*, void*, void*, int*); 
void ATR L_ACCUMULATE_1D(void*, void*, void*, int*); 
void ATR D_ACCUMULATE_1D(void*, void*, void*, int*); 
void ATR C_ACCUMULATE_1D(void*, void*, void*, int*); 
void ATR Z_ACCUMULATE_1D(void*, void*, void*, int*); 


#ifndef CRAY_T3E
void ATR F_ACCUMULATE_2D(void*, int*, int*, void*, int*, void*, int*);
void ATR F_ACCUMULATE_1D(void*, void*, void*, int*); 
#else
#define F_ACCUMULATE_2D F_ACCUMULATE_2D_
#define F_ACCUMULATE_1D F_ACCUMULATE_1D_
void F_ACCUMULATE_2D_(void*, int*, int*, void*, int*, void*, int*);
void F_ACCUMULATE_1D_(void*, void*, void*, int*); 
#endif

extern void armci_acc_2D(int op, void* scale, int proc, void *src_ptr, 
                         void *dst_ptr, int bytes, 
		         int cols, int src_stride, int dst_stride, int lockit); 

#endif
