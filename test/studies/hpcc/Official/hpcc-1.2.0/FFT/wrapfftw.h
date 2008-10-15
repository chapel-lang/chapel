
#ifdef USING_FFTW

#include <fftw.h>

#else

typedef double fftw_real;
typedef struct {
     fftw_real re, im;
} fftw_complex_orig;
typedef fftw_real HPCC_Complex[2];
typedef HPCC_Complex fftw_complex;

typedef enum {
     FFTW_FORWARD = -1, FFTW_BACKWARD = 1
} fftw_direction;
#endif

struct hpcc_fftw_plan_struct {
  fftw_complex *w1, *w2, *ww, *c;
  int n, c_size;
  int flags;
  fftw_direction dir;
};
typedef struct hpcc_fftw_plan_struct *hpcc_fftw_plan;

extern hpcc_fftw_plan HPCC_fftw_create_plan(int n, fftw_direction dir, int flags);
extern void HPCC_fftw_destroy_plan(hpcc_fftw_plan plan);
extern void HPCC_fftw_one(hpcc_fftw_plan plan, fftw_complex *in, fftw_complex *out);

#ifndef USING_FFTW

typedef struct hpcc_fftw_plan_struct *fftw_plan;

#define c_re(c)  ((c)[0])
#define c_im(c)  ((c)[1])

#define fftw_malloc malloc
#define fftw_free free
/* flags for the planner */
#define  FFTW_ESTIMATE (0)
#define  FFTW_MEASURE  (1)

#define FFTW_OUT_OF_PLACE (0)
#define FFTW_IN_PLACE (8)
#define FFTW_USE_WISDOM (16)

#define fftw_create_plan HPCC_fftw_create_plan
#define fftw_destroy_plan HPCC_fftw_destroy_plan
#define fftw_one HPCC_fftw_one

#endif
