{
  use SysCTypes;

  extern proc sizeof(type t): size_t;

  // We just want to check that this gets code
  // generated as sizeof(c_int) vs e.g. sizeof(int32_t);
  var x = sizeof(c_uint);

  // Just using x so it's not dead code
  assert(x>0);
}

