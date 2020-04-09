#include <netcdf.h>

static inline int nc_put_vara_float_WAR(int ncid, int temp_varid, unsigned long long *start, unsigned long long* count, float *temp_out) {
  return nc_put_vara_float(ncid, temp_varid, (unsigned long*)start, (unsigned long*)count, temp_out);
}
