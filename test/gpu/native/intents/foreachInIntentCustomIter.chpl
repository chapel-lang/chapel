use GPU;
use CTypes;
use GpuDiagnostics;

iter myIter() {
  var __CFL_IDX__ : int;
  while __primitive("C for loop",
          __primitive( "=", __CFL_IDX__, 0),
          __primitive("<=", __CFL_IDX__, 0),
          __primitive("+=", __CFL_IDX__, 1))
  {
    yield __CFL_IDX__;
  }

  while __primitive("C for loop",
          __primitive( "=", __CFL_IDX__, 1),
          __primitive("<=", __CFL_IDX__, 1),
          __primitive("+=", __CFL_IDX__, 1))
  {
    yield __CFL_IDX__;
  }
}

// --------------

proc doit() {
  writeln("ON CPU ---");
  var __VAR__ = 100;
  writeln("BEFORE ALL: ", __VAR__);
  foreach __IDX__ in myIter() with (in __VAR__) {
    writeln("BEFORE: ", __VAR__);
    __VAR__ = __IDX__;
    writeln("AFTER: ", __VAR__);
  }
  writeln("AFTER ALL: ", __VAR__);

  writeln();

  on here.gpus[0] {
    writeln("ON GPU ---");
    var __VAR__ = 100;
    writeln("BEFORE ALL: ", __VAR__);
    foreach __IDX__ in myIter() with (in __VAR__) {
      gpuWritef("BEFORE: %i\n":c_ptrConst(c_char), __VAR__);
      __VAR__ = __IDX__;
      gpuWritef("AFTER: %i\n":c_ptrConst(c_char), __VAR__);
    }
    writeln("AFTER ALL: ", __VAR__);
  }
}

doit();
