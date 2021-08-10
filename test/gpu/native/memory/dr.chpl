use CPtr;

extern proc chpl_gpu_init(): void;

pragma "always resolve function"
pragma "codegen for GPU"
export proc setIndex(ptr: c_ptr(int), i: int) {
  ptr[0] = i*10+5;
}

config const n = 5;

on here.getChild(1) {
  chpl_gpu_init();

  var a: [0..#n] int;

  for i in a.domain {
    __primitive("gpu kernel launch flat", c"setIndex", 1, 1, c_ptrTo(a[i]), i);
  }

  // We are able to do this because this test was written with the assumption
  // that we use the unified memory for GPU allocations. That memory can be
  // addressed by the CPU, which is what happens when running this writeln. If
  // we stop doing that (likely), the writeln will need to change.
  writeln(a);
}
