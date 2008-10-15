#ifdef USING_FFTW
#include <fftw_mpi.h>
#else
#include <mpi.h>
typedef struct hpcc_fftw_mpi_plan_struct *fftw_mpi_plan;
#define fftw_mpi_create_plan  HPCC_fftw_mpi_create_plan
#define fftw_mpi_destroy_plan HPCC_fftw_mpi_destroy_plan
#define fftw_mpi HPCC_fftw_mpi
#define fftw_mpi_local_sizes HPCC_fftw_mpi_local_sizes
#endif

struct hpcc_fftw_mpi_plan_struct {
  MPI_Comm comm;
  MPI_Datatype cmplx;
  fftw_complex *wx, *wy, *wz, *c, *work;
  s64Int_t n;
  int flags, c_size;
  fftw_direction dir;
  double *timings;
};
typedef struct hpcc_fftw_mpi_plan_struct *hpcc_fftw_mpi_plan;

extern hpcc_fftw_mpi_plan
HPCC_fftw_mpi_create_plan(MPI_Comm comm, s64Int_t n, fftw_direction dir, int flags);
extern void HPCC_fftw_mpi_destroy_plan(hpcc_fftw_mpi_plan plan);
extern void HPCC_fftw_mpi(hpcc_fftw_mpi_plan p, int n_fields, fftw_complex *local_data,
                     fftw_complex *work);
extern void HPCC_fftw_mpi_local_sizes(hpcc_fftw_mpi_plan p, s64Int_t *local_n,
              s64Int_t *local_start, s64Int_t *local_n_after_transform,
              s64Int_t *local_start_after_transform, s64Int_t *total_local_size);

extern int
HPCC_pzfft1d(s64Int_t n, fftw_complex *a, fftw_complex *b, fftw_complex *w, int me, int npu, int iopt,
  hpcc_fftw_mpi_plan p);

extern double *HPCC_fft_timings_forward, *HPCC_fft_timings_backward;
