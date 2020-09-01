use Random;
use LinearAlgebra;
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

 // test_gbmv();
  test_gemv();
  test_ger();
  test_geru();
}

proc test_gbmv() {
//  test_cugbmv_helper(real(32));
//  test_cugbmv_helper(real(64));
//  test_cugbmv_helper(complex(64));
//  test_cugbmv_helper(complex(128));
}

proc test_gemv() {
  test_cugemv_helper(real(32));
  test_cugemv_helper(real(64));
  test_cugemv_helper(complex(64));
  test_cugemv_helper(complex(128));
}

proc test_ger() {
  test_cuger_helper(real(32));
  test_cuger_helper(real(64));
}

proc test_geru() {
  test_cugeru_helper(complex(64));
  test_cugeru_helper(complex(128));
}


proc test_cugemv_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgemv".format(blasPrefix(t));

  // Simple test
  {

    const m = 10 : c_int;

    // Square
    var A : [{0.. #m, 0.. #m}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    //Get pointer to X allocated on GPU
    var gpu_ptr_A = cpu_to_gpu(c_ptrTo(A), c_sizeof(t)*(m*m):size_t);
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*m:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*m:size_t);

    var alpha = rng.getNext(): t;
    var beta = rng.getNext(): t;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[i,..]*X[..]));

    select t {
      when real(32) do { 
        cu_sgemv(cublas_handle, 1, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when real(64) do {
        cu_dgemv(cublas_handle, 1, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when complex(64) do {
        cu_cgemv(cublas_handle, 1, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when complex(128) do {
        cu_zgemv(cublas_handle, 1, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }

    }


    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*m:size_t);
    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }

  // Try transposing A
  {

    const m = 10 : c_int;

    // Square
    var A : [{0.. #m, 0.. #m}]t,
        X : [{0.. #m}]t,
        Y : [{0.. #m}]t,
        R : [{0.. #m}]t; // Result

    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);

    //Get pointer to X allocated on GPU
    var gpu_ptr_A = cpu_to_gpu(c_ptrTo(A), c_sizeof(t)*(m*m):size_t);
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*m:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*m:size_t);

    var alpha = rng.getNext(): t;
    var beta = rng.getNext(): t;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    // Compute Result vector
    for i in R.domain do R[i] = beta*Y[i] + alpha*(+ reduce (A[..,i]*X[..]));

    select t {
      when real(32) do {
        cu_sgemv(cublas_handle, 0, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when real(64) do {
        cu_dgemv(cublas_handle, 0, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when complex(64) do {
        cu_cgemv(cublas_handle, 0, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
      when complex(128) do {
        cu_zgemv(cublas_handle, 0, m, m, alpha, gpu_ptr_A:c_ptr(t), m, gpu_ptr_X:c_ptr(t), beta, gpu_ptr_Y:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*m:size_t);
    var err = max reduce abs(Y-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }

  printErrors(name, passed, failed, tests);
}

proc test_cuger_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sger".format(blasPrefix(t));

  // Simple test
  {

    // Square case
    const m = 10 : c_int,
          n = 10 : c_int;

    var A : [{0.. #m, 0.. #n}] t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);
    var alpha = rng.getNext();

    //Get pointer to X allocated on GPU
    var gpu_ptr_A = cpu_to_gpu(c_ptrTo(A), c_sizeof(t)*(m*n):size_t);
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*m:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*m:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[j, i];

    select t {
      when real(32) do {
        cu_sger(cublas_handle, m, n, alpha, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), gpu_ptr_A:c_ptr(t), m);
      }
      when real(64) do {
        cu_dger(cublas_handle, m, n, alpha, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), gpu_ptr_A:c_ptr(t), m);
      }
   }

    gpu_to_cpu(c_ptrTo(A), gpu_ptr_A, c_sizeof(t)*(m*n):size_t);
    var err = max reduce abs(transpose(A)-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}

proc test_cugeru_helper(type t){
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sgeru".format(blasPrefix(t));
  // Simple test
  {
    // Square case
    const m = 10 : c_int,
          n = 10 : c_int;

    var A : [{0.. #m, 0.. #n}] t,
        X : [{0.. #m}]t,
        Y : [{0.. #n}]t,
        R : [{0.. #m, 0..#n}] t; // Result


    // Populate values
    var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(X);
    rng.fillRandom(Y);
    var alpha = rng.getNext();

    //Get pointer to X allocated on GPU
    var gpu_ptr_A = cpu_to_gpu(c_ptrTo(A), c_sizeof(t)*(m*n):size_t);
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*m:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*m:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    // Precompute result
    for (i,j) in R.domain do R[i, j] = alpha*X[i]*Y[j] + A[j, i];

    select t {
      when complex(64) do {
        cu_cgeru(cublas_handle, m, n, alpha, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), gpu_ptr_A:c_ptr(t), m);
      }
      when complex(128) do {
        cu_zgeru(cublas_handle, m, n, alpha, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), gpu_ptr_A:c_ptr(t), m);
      }
   }

    gpu_to_cpu(c_ptrTo(A), gpu_ptr_A, c_sizeof(t)*(m*n):size_t);
    var err = max reduce abs(transpose(A)-R);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}
