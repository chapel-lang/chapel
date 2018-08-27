#include <stdlib.h>
#include <netcdf.h>

/* Work around a problem with 'size_t*' being 'unsigned long*' vs.
   in different places 'unsigned long long*'
 */
static int nc_inq_dimlen_WAR(int ncid, int dimid, void* dimlens) {
  return nc_inq_dimlen(ncid, dimid, (size_t*)dimlens);
}


/* The functions below were used to verify bitwise equality between
   the results computed by compOcean.chpl and the python version.
 */
char hexstr[19];

static const char* float_to_hex(float x) {
  sprintf(hexstr, "0x%8x", *(unsigned int*)&x);
  return hexstr;
}

static const char* double_to_hex(double x) {
  sprintf(hexstr, "0x%.8x%.8x", ((unsigned int*)&x)[1], ((unsigned int*)&x)[0]);
  return hexstr;
}
