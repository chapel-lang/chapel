use CPtr;

extern proc chpl_gpu_init(): void;

pragma "always resolve function"
pragma "codegen for GPU"
export proc setIndex(ptr: c_ptr(int)) {
  ptr[2] = 2;
}

config const n = 5;

on here.getChild(1) {
  chpl_gpu_init();

  var a: [0..#n] int;

  for i in a.domain {
    __primitive("gpu kernel launch flat", c"setIndex", 1, 1, 1, c_ptrTo(a[0]));
  }

  writeln(a);
}
