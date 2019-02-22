#ifdef __ICC
#include "ISO_Fortran_binding.h"
#else
#ifdef _CRAYC
#include "ISO_Fortran_binding.h"
#else
// Stub it out for other compilers
#endif
#endif
