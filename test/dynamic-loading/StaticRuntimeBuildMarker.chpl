use CTypes;

// Variable that is defined in the runtime in 'chpl-prginfo.h'.
proc main() {
  extern const chpl_rt_is_dynamic_library: c_int;
  var ok = chpl_rt_is_dynamic_library == 0;
  assert(ok);
}
