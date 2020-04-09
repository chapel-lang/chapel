#include "selects.h"

lapack_logical LAPACK_Z_SELECT1_LESS_THAN( const lapack_complex_double* a ){
  double real_part = lapack_complex_double_real( *a );
  double imag_part = lapack_complex_double_imag( *a );
  return (lapack_logical) (real_part*real_part + imag_part*imag_part < 2 );
}

lapack_logical LAPACK_D_SELECT2_LESS_THAN( const double* a, const double* b){
  return (lapack_logical) (*a < *b );
}
