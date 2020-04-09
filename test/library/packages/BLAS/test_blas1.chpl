use BLAS;
use blas_helpers;
use IO;

proc main() {
  test_rotg();
  test_rotmg();
  test_rot();
  test_rotm();
  test_swap();
  test_scal();
  test_copy();
  test_axpy();
  test_dot();
  test_dotu_sub();
  test_dotc_sub();
  test_dsdot();
  // Not tested due to compatibility issue with CrayBLAS
  //test_sdsdot();
  test_nrm2();
  test_asum();
  test_amax();
}

proc test_rotg() {
  test_rotg_helper(real(32));
  test_rotg_helper(real(64));
}

proc test_rotmg() {
  test_rotmg_helper(real(32));
  test_rotmg_helper(real(64));
}

proc test_rot() {
  test_rot_helper(real(32));
  test_rot_helper(real(64));
}

proc test_rotm() {
  test_rotm_helper(real(32));
  test_rotm_helper(real(64));
}

proc test_swap() {
  test_swap_helper(real(32));
  test_swap_helper(real(64));
  test_swap_helper(complex(64));
  test_swap_helper(complex(128));
}

proc test_scal() {
  test_scal_helper(real(32));
  test_scal_helper(real(64));
  test_scal_helper(complex(64));
  test_scal_helper(complex(128));
}

proc test_copy() {
  test_copy_helper(real(32));
  test_copy_helper(real(64));
  test_copy_helper(complex(64));
  test_copy_helper(complex(128));
}

proc test_axpy() {
  test_axpy_helper(real(32));
  test_axpy_helper(real(64));
  test_axpy_helper(complex(64));
  test_axpy_helper(complex(128));
}

proc test_dot() {
  test_dot_helper(real(32));
  test_dot_helper(real(64));
}

proc test_dotu_sub() {
  test_dotu_helper(complex(64));
  test_dotu_helper(complex(128));
}

proc test_dotc_sub() {
  test_dotc_helper(complex(64));
  test_dotc_helper(complex(128));
}

/* Name breaks convention, since only real(64) accepted */
proc test_dsdot() {
  test_dsdot_helper();
}

/* Name breaks convention, since only real(32) accepted*/
proc test_sdsdot() {
  test_sdsdot_helper();
}

proc test_nrm2() {
  test_nrm2_helper(real(32));
  test_nrm2_helper(real(64));
  test_nrm2_helper(complex(64));
  test_nrm2_helper(complex(128));
}

proc test_asum() {
  test_asum_helper(real(32));
  test_asum_helper(real(64));
  test_asum_helper(complex(64));
  test_asum_helper(complex(128));
}

proc test_amax() {
  test_amax_helper(real(32));
  test_amax_helper(real(64));
  test_amax_helper(complex(64));
  test_amax_helper(complex(128));
}


proc test_rotg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotg".format(blasPrefix(t));

  // Simple test
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

    rotg(a, b, c, s);

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

proc test_rotmg_helper(type t) {
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

    const X1 = b1*sqrt(d1),
          Y1 = b2*sqrt(d2);

    // Param array
    var P: [0..4] t;

    rotmg(d1, d2, b1, b2, P);

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


proc test_rot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srot".format(blasPrefix(t));

  // Simple test
  {
    var err: t;

    const D = {0..1};

    var X: [D] t = [4:t, 2:t],
        Y: [D] t = [1:t, 3:t],
        c = 2.0: t,
        s = 2.0: t;

    const Xin = X,
          Yin = Y;

    rot(X, Y, c, s);

    for i in X.domain {

      err = abs(Xin[i] * c  + Yin[i] * s - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);

      err = abs(Yin[i] * c  - Xin[i] * s - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }

  }
  printErrors(name, passed, failed, tests);
}


proc test_rotm_helper(type t) {
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

    rotm(X, Y, P);

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

proc test_swap_helper(type t) {
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

    swap(X, Y);

    for i in D {
      var err = abs(Xin[i] - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
      err = abs(Yin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}


proc test_scal_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sscal".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};

    var a = 1.5: t,
        X: [D] t = [1: t, 2: t, 3: t];

    const Xin = X;

    scal(X, a);

    for i in D {
      var err = abs(a * Xin[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_copy_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%scopy".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];
    var Y: [D] t;

    copy(X, Y);

    for i in D {
      var err = abs(Y[i] - X[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_axpy_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%saxpy".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var a = 2: t;

    const Yin = Y;

    axpy(X, Y, a);

    for i in D {
      var err = abs(a*X[i] + Yin[i] - Y[i]);
      trackErrors(name, err, errorThreshold, passed, failed, tests);
    }
  }
  printErrors(name, passed, failed, tests);
}

proc test_dot_helper(type t) {
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

    var prod = X*Y;
    var red = + reduce prod;

    var res = dot(X, Y);

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_dotu_helper(type t) {
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

    var prod = X*Y;
    var red = + reduce prod;

    var res = dotu(X, Y);

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_dotc_helper(type t) {
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

    var prod = conjg(X)*Y;
    var red = + reduce prod;

    var res = dotc(X, Y);

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_dsdot_helper() {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(real(64));
  var name = "dsdot";

  type t = real(32);
  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var prod = X*Y;
    var red = (+ reduce prod): real(64);

    var res = dsdot(X, Y);

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}

proc test_sdsdot_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "sdsdot";

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t],
        Y: [D] t = [3: t, 2: t, 1: t];

    var prod = X*Y;
    var red = + reduce prod;

    var res = sdsdot(X, Y);

    var err = abs(red - res);
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }

  printErrors(name, passed, failed, tests);
}

proc test_nrm2_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%snrm2".format(blasPrefix(t));

  // Simple test

  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];

    const norm = sqrt(+ reduce (abs(X)*abs(X)));

    var res = nrm2(X);

    var err = norm - res;
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_asum_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%sasum".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [1: t, 2: t, 3: t];

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

    var res = asum(X);

    var err = norm - res;
    trackErrors(name, err, errorThreshold, passed, failed, tests);
  }
  printErrors(name, passed, failed, tests);
}

proc test_amax_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%samax".format(blasPrefix(t));

  // Simple test
  {
    const D = {0..2};
    var X: [D] t = [3: t, 2: t, 4: t];

    var idx = amax(X);

    var err = abs(idx - 2);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }

  // Non-zero based domain
  {
    const D = {1..3};
    var X: [D] t = [3: t, 2: t, 4: t];

    var idx = amax(X);

    var err = abs(idx - 3);
    trackErrors(name, err, errorThreshold, passed, failed, tests);

  }
  printErrors(name, passed, failed, tests);
}
