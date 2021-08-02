use CPtr;

pragma "codegen for GPU"
pragma "always resolve function"
export proc sample_kernel(dst_ptr: c_ptr(real(64))) {
  dst_ptr[0] = __primitive("gpu threadIdx x");
}

