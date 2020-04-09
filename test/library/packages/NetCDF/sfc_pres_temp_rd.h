#include <netcdf.h>

// This is a wrapper function that takes a void* argument and casts it to
// char* to call the wrapped function.
static inline int nc_get_att_text_void_ptr(int ncid, int varid, const char* field, void* p) {
  return nc_get_att_text(ncid, varid, field, (char*)p);
}
