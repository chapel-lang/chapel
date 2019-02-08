module common {
  extern {
    #include "common.h"

    static double* _getPtr(MATRIX_type* x) {
      return &(x->matrix);
    }
  }
  
  proc getPtr(ref val : MATRIX_type) {
    return _getPtr(c_ptrTo(val));
  }
}
