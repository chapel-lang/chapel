use CTypes;

extern {
  #define MACRO_SCALE(x) ((x) * 3 + 1)
  #define MACRO_OFFSET ((unsigned int)(sizeof(int) + 1))
}

pragma "codegen for GPU"
extern proc MACRO_SCALE(x: c_int): c_int;
extern const MACRO_OFFSET: c_uint;

on here.gpus[0] {
  var values: [0..<16] c_int;
  @assertOnGpu
  foreach i in values.domain {
    values[i] = MACRO_SCALE(i:c_int) + MACRO_OFFSET:c_int;
  }
  for i in values.domain do
    assert(values[i] == 3 * i + 6);
}
writeln("GPU macros passed");
