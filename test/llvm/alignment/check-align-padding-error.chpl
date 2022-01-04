extern {
  #include <stdio.h>
  #include <stdbool.h>
  #include <inttypes.h>
  #include <complex.h>
  #include <string.h>

  struct aligned16test {
    uint64_t x;
    uint64_t a __attribute__((aligned (16)));
  };
}

record int8int64 {
  var field1: int(8);
  var field2: int;
}
record int8int32 {
  var field1: int(8);
  var field2: int(32);
}
record int32int64 {
  var field1: int(32);
  var field2: int(64);
}
record int8int8int64 {
  var field1: int(8);
  var field2: int(8);
  var field3: int(64);
}
record int8int16int64 {
  var field1: int(8);
  var field2: int(16);
  var field3: int(64);
}
record int64aligned16b {
  var field1: int;
  var field2: aligned16test;
}

proc compute(arg) {
  writeln(arg);
}

proc main() {
  var v1: int8int64;
  compute(v1);
  var v2: int8int32;
  compute(v2);
  var v3: int32int64;
  compute(v3);
  var v4: int8int8int64;
  compute(v4);
  var v5: int8int16int64;
  compute(v5);
  var v6: int64aligned16b;
  compute(v6);
}
