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
  test_axpy();
  test_copy();
  test_dot();
  test_dotu();
  test_dotc();
  test_nrm2();
  test_rot();
  test_rotg();
  test_rotm();
  test_rotmg();
  test_scal();
  test_swap();

}

proc test_amax() {
  test_cuamax_helper(real(32));
  test_cuamax_helper(real(64));
  test_cuamax_helper(complex(64));
  test_cuamax_helper(complex(128));
}

proc test_amin() {
  test_cuamin_helper(real(32));
  test_cuamin_helper(real(64));
  test_cuamin_helper(complex(64));
  test_cuamin_helper(complex(128));
}

proc test_asum() {
  test_cuasum_helper(real(32));
  test_cuasum_helper(real(64));
  test_cuasum_helper(complex(64));
  test_cuasum_helper(complex(128));
}

proc test_axpy() {
  test_cuaxpy_helper(real(32));
  test_cuaxpy_helper(real(64));
  test_cuaxpy_helper(complex(64));
  test_cuaxpy_helper(complex(128));
}

proc test_copy() {
  test_cucopy_helper(real(32));
  test_cucopy_helper(real(64));
  test_cucopy_helper(complex(64));
  test_cucopy_helper(complex(128));
}

proc test_dot() {
  test_cudot_helper(real(32));
  test_cudot_helper(real(64));
}

proc test_dotu(){
  test_cudotu_helper(complex(64));
  test_cudotu_helper(complex(128));
}

proc test_dotc(){
  test_cudotc_helper(complex(64));
  test_cudotc_helper(complex(128));
}

proc test_nrm2() {
  test_cunrm2_helper(real(32));
  test_cunrm2_helper(real(64));
  test_cunrm2_helper(complex(64));
  test_cunrm2_helper(complex(128));
}

proc test_rot() {
  test_curot_helper(real(32));
  test_curot_helper(real(64));
  test_cucrot_helper(complex(64));
  test_cucsrot_helper(complex(64));
  test_cuzrot_helper(complex(128));
  test_cuzdrot_helper(complex(128));
}

proc test_rotg() {
  test_curotg_helper(real(32));
  test_curotg_helper(real(64));
  test_cucrotg_helper(complex(64));
  test_cuzrotg_helper(complex(128));
}

proc test_rotm() {
  test_curotm_helper(real(32));
  test_curotm_helper(real(64));
}

proc test_rotmg() {
  test_curotmg_helper(real(32));
  test_curotmg_helper(real(64));
}

proc test_scal(){
  test_cuscal_helper(real(32));
  test_cuscal_helper(real(64));
  test_cuscal_helper(complex(64));
  test_cucsscal_helper(complex(64));
  test_cuscal_helper(complex(128));
  test_cuzdscal_helper(complex(128));
}

proc test_swap(){
  test_cuswap_helper(real(32));
  test_cuswap_helper(real(64));
  test_cuswap_helper(complex(64));
  test_cuswap_helper(complex(128));
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

    //Get pointer to X allocated on GPU
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
      when complex(64) do {
        cu_icamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when complex(128) do {
        cu_izamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
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
      when complex(64) do {
        cu_icamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when complex(128) do {
        cu_izamax(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r;
    var err = abs(idx - 3);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
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

    //Get pointer to X allocated on GPU
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
      when complex(64) do {
        cu_icamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when complex(128) do {
        cu_izamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
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

    //Get pointer to X allocated on GPU
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
      when complex(64) do {
        cu_icamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
      when complex(128) do {
        cu_izamin(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
      }
    }

    var idx = r;
    var err = abs(idx - 2);
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

    //Get pointer to X allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    var err = 1.0;

    select t {
      when real(32) do {
        var r : real(32);
        cu_sasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
        err = norm - r;
      }
      when real(64) do {
        var r : real(64);
        cu_dasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
        err = norm - r;
      }
      when complex(64) do {
        var r : real(32);
        cu_scasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
        err = norm - r;
      }
      when complex(128) do {
        var r : real(64);
        cu_dzasum(cublas_handle, N, gpu_ptr_X:c_ptr(t), 1, c_ptrTo(r));
        err = norm - r;
      }
    }

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

    //Get pointer to X and Y allocated on GPU
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
      when complex(64) do {
        cu_ccopy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
      when complex(128) do {
        cu_zcopy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
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

    //Get pointer to X and Y allocated on GPU
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
      when complex(64) do {
        cu_caxpy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), a);
      }
      when complex(128) do {
        cu_zaxpy(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), a);
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

  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
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

proc test_cudotu_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sdotu".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];
    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    var prod = X*Y;
    var red = + reduce prod;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();
    var res : t;

    select t {
      when complex(64) do {
        cu_cdotu(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(res));
      }
      when complex(128) do {
        cu_zdotu(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(res));
      }
    }

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cudotc_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sdotc".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];
    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    var prod = conjg(X)*Y;
    var red = + reduce prod;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();
    var res : t;

    select t {
      when complex(64) do {
        cu_cdotc(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(res));
      }
      when complex(128) do {
        cu_zdotc(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c_ptrTo(res));
      }
    }

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cunrm2_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%snrm2".format(blasPrefix(t));

  // Simple test

  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];

    var N = X.size:int(32);

    const norm = sqrt(+ reduce (abs(X)*abs(X)));

    //Get pointer to X allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();
    var err = 1.0;

    select t {
      when real(32) do {
        var r: real(32);
        cu_snrm2(cublas_handle, N, gpu_ptr_X:c_ptr(t), c_ptrTo(r));
        err = norm - r;
      }
      when real(64) do {
        var r: real(64);
        cu_dnrm2(cublas_handle, N, gpu_ptr_X:c_ptr(t), c_ptrTo(r));
        err = norm - r;
      }
      when complex(64) do {
        var r: real(32);
        cu_scnrm2(cublas_handle, N, gpu_ptr_X:c_ptr(t), c_ptrTo(r));
        err = norm -r;
      }
      when complex(128) do {
        var r: real(64);
        cu_dznrm2(cublas_handle, N, gpu_ptr_X:c_ptr(t), c_ptrTo(r));
        err = norm - r;
      }
    }

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_curot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srot".format(blasPrefix(t));

  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t],
        c = 2.0: t,
        s = 2.0: t;

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_srot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
      }
      when real(64) do {
        cu_drot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in X.domain {
      err = abs(Xin[i] * c + Yin[i] * s - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c - Xin[i] * s - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }

  }
  printErrors(name, passed, failed, tests);
}

proc test_cucrot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%scrot".format(blasPrefix(t));

  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t],
        c = 2.0: real(32),
        s = 2.0: t;

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
     when complex(64) do {
        cu_crot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
     }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in X.domain {
      err = abs(Xin[i] * c + Yin[i] * s - X[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c - Xin[i] * s - Y[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    }

  }
  printErrors(name, passed, failed, tests);
}

proc test_cuzrot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srot".format(blasPrefix(t));

  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t],
        c = 2.0: real(64),
        s = 2.0: t;

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
     when complex(128) do {
        cu_zrot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
     }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in X.domain {
      err = abs(Xin[i] * c + Yin[i] * s - X[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c - Xin[i] * s - Y[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    }

  }
  printErrors(name, passed, failed, tests);
}

proc test_cucsrot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssrot".format(blasPrefix(t));

  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t];
    var c = 2.0: real(32);
    var s = 2.0: real(32);

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
     when complex(64) do {
        cu_csrot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
     }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in X.domain {

      err = abs(Xin[i] * c  + Yin[i] * s - X[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c - Xin[i] * s - Y[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    }

  }
  printErrors(name, passed, failed, tests);
}

proc test_cuzdrot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sdrot".format(blasPrefix(t));

  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t];
    var c = 2.0: real(64);
    var s = 2.0: real(64);

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
     when complex(128) do {
        cu_zdrot(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), c, s);
     }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in X.domain {

      err = abs(Xin[i] * c  + Yin[i] * s - X[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c - Xin[i] * s - Y[i]);
      trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

    }

  }
  printErrors(name, passed, failed, tests);
}

proc test_curotg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotg".format(blasPrefix(t));

  {

    var err: t;

    // inputs
    var a = 1.0: t,
        b = 0.0: t;
    // outputs
    var c: t,
        s: t;

    // Save inputs by value
    var A: t = a,
        B: t = b;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_srotg(cublas_handle, a, b, c, s);
      }
      when real(64) do {
        cu_drotg(cublas_handle, a, b, c, s);
      }
    }

    // rename outputs
    var r = a;
    var z = b;
    const zero: t;

    // r == sqrt(a**2 + b**2)
    var R = ((A**2 + B**2)**0.5): t;
    err = abs(r - R);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    // c s * a  = r
    //-s c   b    0
    err = abs(c*A + s*B - r);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
    err = abs(-s*A + c*B);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    // The parameter z is defined such that if |a| > |b|, z is s; otherwise if
    // c is not 0 z is 1/c; otherwise z is 1.
    if abs(A) > abs(B) then
      err = abs(z - s);
    else if c != zero then
      err = abs((z - 1.0/c): t);
    else
      err = abs((z - 1.0): t);

    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cucrotg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotg".format(blasPrefix(t));

  {

    var err: t;

    // inputs
    var a = 1.0: t,
        b = 0.0: t;
    // outputs
    var c: real(32),
        s: t;

    // Save inputs by value
    var A: t = a,
        B: t = b;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when complex(64) do {
        cu_crotg(cublas_handle, a, b, c, s);
      }
    }

    // rename outputs
    var r = a;
    var z = b;
    const zero: t;

    // r == sqrt(a**2 + b**2)
    var R = ((A**2 + B**2)**0.5): t;
    err = abs(r - R);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    // c s * a  = r
    //-s c   b    0
    err = abs(c*A + s*B - r);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    err = abs(-s*A + c*B);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

    // The parameter z is defined such that if |a| > |b|, z is s; otherwise if
    // c is not 0 z is 1/c; otherwise z is 1.
    if abs(A) > abs(B) then
      err = abs(z - s);
    else if c != zero then
      err = abs((z - 1.0/c): t);
    else
      err = abs((z - 1.0): t);

    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuzrotg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotg".format(blasPrefix(t));

  {

    var err: t;

    // inputs
    var a = 1.0: t,
        b = 0.0: t;
    // outputs
    var c: real(64),
        s: t;

    // Save inputs by value
    var A: t = a,
        B: t = b;

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when complex(128) do {
        cu_zrotg(cublas_handle, a, b, c, s);
      }
    }

    // rename outputs
    var r = a;
    var z = b;
    const zero: t;

    // r == sqrt(a**2 + b**2)
    var R = ((A**2 + B**2)**0.5): t;
    err = abs(r - R);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    // c s * a  = r
    //-s c   b    0
    err = abs(c*A + s*B - r);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
    err = abs(-s*A + c*B);
    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);

    // The parameter z is defined such that if |a| > |b|, z is s; otherwise if
    // c is not 0 z is 1/c; otherwise z is 1.
    if abs(A) > abs(B) then
      err = abs(z - s);
    else if c != zero then
      err = abs((z - 1.0/c): t);
    else
      err = abs((z - 1.0): t);

    trackErrors(name, abs(err), errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_curotm_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotm".format(blasPrefix(t));
  var err: t;

  // Simple test
  {
    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t];

    var P: [0..4] t = [-2.0: t, // flag
                        1.0: t, 0.0: t,
                        0.0: t, 1.0: t];

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y and P allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_srotm(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), P);
      }
      when real(64) do {
        cu_drotm(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t), P);
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    var flag = P[0],
        h11 = P[1],
        h21 = P[2],
        h12 = P[3],
        h22 = P[4];

    select flag {
      when 0.0: t do {
        h11 = 1.0: t;
        h22 = 1.0: t;
      }
      when 1.0: t do {
        h12 = 1.0: t;
        h21 = -1.0: t;
      }
      when -2.0: t do {
        h11 = 1.0: t;
        h22 = 1.0: t;
        h12 = 0.0: t;
        h21 = 0.0: t;
      }
    }

    for i in D {
      err = abs(h11 * Xin[i] + h12 * Yin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
      err = abs(h21 * Xin[i] + h22 * Yin[i] - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_curotmg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotmg".format(blasPrefix(t));

  // Simple test
  {
    var err: t;

    var d1 = 1.0: t,
        d2 = 1.0: t,
        b1 = 1.0: t,
        b2 = 1.0: t;

    var X1 = b1*sqrt(d1) : t;
    var Y1 = b2*sqrt(d2) : t;

    //writeln("X1: ", X1, "Y1: ", Y1);

    // Param array
    var P: [0..4] t;

    var N = 1:int(32);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_srotmg(cublas_handle, d1, d2, b1, b2, P);
      }
      when real(64) do {
        cu_drotmg(cublas_handle, d1, d2, b1, b2, P);
      }
    }

    var flag = P[0],
        h11 = P[1],
        h21 = P[2],
        h12 = P[3],
        h22 = P[4];

    select flag {
      when 0.0: t do {
        h11 = 1.0: t;
        h22 = 1.0: t;
      }
      when 1.0: t do {
        h12 = 1.0: t;
        h21 = -1.0: t;
      }
      when -2.0: t do {
        h11 = 1.0: t;
        h22 = 1.0: t;
        h12 = 0.0: t;
        h21 = 0.0: t;
      }
    }

    var x1 = b1;

    err = abs(h11*X1 + h12*Y1 - x1);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

    err = abs(h21*X1 + h22*Y1);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuscal_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sscal".format(blasPrefix(t));

  {
    const D = {0..2};

    var a = 1.5: t,
        X: [D] t = [1: t, 2: t, 3: t];

    const Xin = X;
    var N = X.size:int(32);

    //Get pointer to X and Y and P allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_sscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
      when real(64) do {
        cu_dscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
      when complex(64) do {
        cu_cscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
      when complex(128) do {
        cu_zscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(a * Xin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_cucsscal_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%ssscal".format(blasPrefix(t));

  {
    const D = {0..2};

    var a = 1.5: real(32);
    var X: [D] t = [1: t, 2: t, 3: t];

    const Xin = X;
    var N = X.size:int(32);

    //Get pointer to X and Y and P allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when complex(64) do {
        cu_csscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(a * Xin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuzdscal_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sdscal".format(blasPrefix(t));

  {
    const D = {0..2};

    var a = 1.5: real(64);
    var X: [D] t = [1: t, 2: t, 3: t];

    const Xin = X;
    var N = X.size:int(32);

    //Get pointer to X and Y and P allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when complex(128) do {
        cu_zdscal(cublas_handle, N, a, gpu_ptr_X:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(a * Xin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_cuswap_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sswap".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 1: t, 1: t],
        Y: [D] t = [2: t, 2: t, 2: t];

    const Xin = X,
          Yin = Y;

    var N = X.size:int(32);

    //Get pointer to X and Y and P allocated on GPU
    var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(t)*N:size_t);
    var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(t)*N:size_t);

    //Create cublas handle
    var cublas_handle = cublas_create_handle();

    select t {
      when real(32) do {
        cu_sswap(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
      when real(64) do {
        cu_dswap(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
      when complex(64) do {
        cu_cswap(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
      when complex(128) do {
        cu_zswap(cublas_handle, N, gpu_ptr_X:c_ptr(t), gpu_ptr_Y:c_ptr(t));
      }
    }

    gpu_to_cpu(c_ptrTo(X), gpu_ptr_X, c_sizeof(t)*N:size_t);
    gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(t)*N:size_t);

    for i in D {
      var err = abs(Xin[i] - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
      err = abs(Yin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}
