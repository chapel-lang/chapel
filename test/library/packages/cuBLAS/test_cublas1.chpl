use cuBLAS;
use IO;

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc printErrors(name: string, passed, failed, tests) {
  writef("%5s : %i PASSED, %i FAILED\n".format(name, passed, failed));
}

proc trackErrors(name, err, errorThreshold, ref passed, ref failed, ref tests) {
  if err > errorThreshold {
    failed += 1;
    writef("%s : Failure on test %i : %r\n", name, tests, err);
  } else {
    passed += 1;
  }
  tests+=1;
}

proc blasError(type t) {
  select t {
    when real(32) do return errorThresholdSingle;
    when real(64) do return errorThresholdDouble;
    when complex(64) do return errorThresholdSingle;
    when complex(128) do return errorThresholdDouble;
  }
}

proc blasPrefix(type t) {
  select t {
    when real(32) do return "s";
    when real(64) do return "d";
    when complex(64) do return "c";
    when complex(128) do return "z";
  }
}

proc main() {

  test_amax();
  test_amin();
  test_asum();
  test_copy();
  test_dot();
  test_axpy();

}

proc test_amax() {
  test_cuamax_helper(real(32));
  test_cuamax_helper(real(64));
}

proc test_amin(){
  test_cuamin_helper(real(32));
  test_cuamin_helper(real(64));
}

proc test_asum(){
  test_cuasum_helper(real(32));
  test_cuasum_helper(real(64));
}

proc test_copy(){
  test_cucopy_helper(real(32));
  test_cucopy_helper(real(64));
}

proc test_dot(){
  test_cudot_helper(real(32));
  test_cudot_helper(real(64));
}

proc test_axpy() {
  test_cuaxpy_helper(real(32));
  test_cuaxpy_helper(real(64));
}

proc test_cuamin_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%samin".format(blasPrefix(t));

  {
    const D = {0..2};
    var X: [D] t = [3: t, 2: t, 4: t];
    var N = X.size:int(32);
    var r: int(32);

    //Get pointer to X allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_isamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when real(64) do {
        cu_idamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r-1;
    var err = abs(idx - 1);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Non-zero based domain
  {
    const D = {1..3};
    var X: [D] t = [3: t, 2: t, 4: t];
    var N = X.size:int(32);
    var r: int(32);

    //Get pointer to X allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_isamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when real(64) do {
        cu_idamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r;
    var err = abs(idx - 2);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}


proc test_cuamax_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%samax".format(blasPrefix(t));

  {
    const D = {0..2};
    var X: [D] t = [3: t, 2: t, 4: t];
    var N = X.size:int(32);
    var r: int(32);

    //Get pointer to X allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_isamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when real(64) do {
        cu_idamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r-1;
    var err = abs(idx - 2);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  // Non-zero based domain
  {
    const D = {1..3};
    var X: [D] t = [3: t, 2: t, 4: t];
    var N = X.size:int(32);
    var r: int(32);

    //Get pointer to X allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_isamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when real(64) do {
        cu_idamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r;
    var err = abs(idx - 3);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuasum_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sasum".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];
    var N = X.size:int(32);

    param width = if isComplexType(t) then numBits(t)/2 else numBits(t);

    var norm: real(width);
    if isComplexType(t) {
      for x in X {
        norm += abs(x.re);
        norm += abs(x.im);
      }
    } else {
      norm = +reduce(X);
    }

    var r: t;

    //Get pointer to X allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_sasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when real(64) do {
        cu_dasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var err = norm - r;
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }
  printErrors(name, passed, failed, tests);
}

proc test_cucopy_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%scopy".format(blasPrefix(t));

  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];
    var Y: [D] t;
    var N = X.size:int(32);

    //Get pointer to X and Y allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_scopy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
      when real(64) do {
        cu_dcopy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(Y[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuaxpy_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%saxpy".format(blasPrefix(t));

  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var N = X.size:int(32);
    var a = 2: t;
    const Yin = Y;

    //Get pointer to X and Y allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_saxpy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), a);
      }
      when real(64) do {
        cu_daxpy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), a);
      }
    }

    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(a*X[i] + Yin[i] - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_cudot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sdot".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var N = X.size:int(32);

    //Get pointer to X and Y allocated in GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    var prod = X*Y;
    var red = + reduce prod;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();
    var r : t;

    select t {
      when real(32) do {
        cu_sdot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(r));
      }
      when real(64) do {
        cu_ddot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(r));
      }
    }

    var err = abs(red - r);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}
