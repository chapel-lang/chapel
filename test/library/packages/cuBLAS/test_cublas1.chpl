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

    //Get pointer to X allocated in GPU
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

