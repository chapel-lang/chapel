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

  writeln(a);
}
