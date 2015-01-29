// ChapelTaskID.chpl
//
module ChapelTaskID {

  // Define a few operations over chpl_taskID_t
  inline proc =(ref a:chpl_taskID_t, b:chpl_taskID_t) {
    __primitive("=", a, b);
  }

  inline proc ==(a: chpl_taskID_t, b: chpl_taskID_t)
    return __primitive("==", a, b);

  inline proc _cast(type t, x: chpl_taskID_t) where t == int(64)
                                                    || t == uint(64)
    return __primitive("cast", t, x);

}
