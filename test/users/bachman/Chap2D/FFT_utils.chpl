use IO;
use FFTW;
use C_FFTW;
use CTypes;
use CopyAggregation;

use parameters;
use domains;
use arrays;
//use compare_fortran;

/* Forward FFT plans */
var plan_f : fftw_plan;
var plan_f1 : fftw_plan;
var plan_f2 : fftw_plan;

/* Backward FFT plans */
var plan_b : fftw_plan;
var plan_b1 : fftw_plan;
var plan_b2 : fftw_plan;



proc set_up_forward_FFTs() {

    var arr2_in : [D] real(rp);
    var arr2_out : [D_hat] complex(cp);
    var arr_nx : [D_nx] real(rp);
    var arr_nx2p : [D_nx2p] complex(cp);
    var arr_ny : [D_ny] complex(cp);

    // Set up 2D version
    if (rp == 64) {
      extern proc fftw_plan_dft_r2c_2d(n0: c_int, n1: c_int, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)), flags: c_uint) : fftw_plan;
      plan_f = fftw_plan_dft_r2c_2d(ny : c_int, nx : c_int, c_ptrTo(arr2_in), c_ptrTo(arr2_out), FFTW_ESTIMATE);
    } else {
      extern proc fftwf_plan_dft_r2c_2d(n0: c_int, n1: c_int, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)), flags: c_uint) : fftw_plan;
      plan_f = fftwf_plan_dft_r2c_2d(ny : c_int, nx : c_int, c_ptrTo(arr2_in), c_ptrTo(arr2_out), FFTW_ESTIMATE);
    }

    // Set up sequential 1D version
    if (rp == 64) {
      // First forward FFT is along the x dimension, which takes an nx-vector as input and returns an nx2p-vector as output
      extern proc fftw_plan_dft_r2c_1d(n0: c_int, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)), flags: c_uint) : fftw_plan;
      plan_f1 = fftw_plan_dft_r2c_1d(nx : c_int, c_ptrTo(arr_nx), c_ptrTo(arr_nx2p), FFTW_ESTIMATE);

    // Second forward FFT is along the y dimension, for which both input and output are length ny
      extern proc fftw_plan_dft_1d(n0: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(complex(cp)), sign : c_int, flags: c_uint) : fftw_plan;
      plan_f2 = fftw_plan_dft_1d(ny : c_int, c_ptrTo(arr_ny), c_ptrTo(arr_ny), -1, FFTW_ESTIMATE);
    } else {
      // First forward FFT is along the x dimension, which takes an nx-vector as input and returns an nx2p-vector as output
      extern proc fftwf_plan_dft_r2c_1d(n0: c_int, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)), flags: c_uint) : fftw_plan;
      plan_f1 = fftwf_plan_dft_r2c_1d(nx : c_int, c_ptrTo(arr_nx), c_ptrTo(arr_nx2p), FFTW_ESTIMATE);

    // Second forward FFT is along the y dimension, for which both input and output are length ny
      extern proc fftwf_plan_dft_1d(n0: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(complex(cp)), sign : c_int, flags: c_uint) : fftw_plan;
      plan_f2 = fftwf_plan_dft_1d(ny : c_int, c_ptrTo(arr_ny), c_ptrTo(arr_ny), -1, FFTW_ESTIMATE);
    }


}

proc set_up_backward_FFTs() {

    var arr2_in : [D_hat] complex(cp);
    var arr2_out : [D] real(rp);
    var arr_nx : [D_nx] real(rp);
    var arr_nx2p : [D_nx2p] complex(cp);
    var arr_ny : [D_ny] complex(cp);

    // Set up 2D version
    if (rp == 64) {
      extern proc fftw_plan_dft_c2r_2d(n0: c_int, n1: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(real(rp)), flags: c_uint) : fftw_plan;
      plan_b = fftw_plan_dft_c2r_2d(ny : c_int, nx : c_int, c_ptrTo(arr2_in), c_ptrTo(arr2_out), FFTW_ESTIMATE);
    } else {
      extern proc fftwf_plan_dft_c2r_2d(n0: c_int, n1: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(real(rp)), flags: c_uint) : fftw_plan;
      plan_b = fftwf_plan_dft_c2r_2d(ny : c_int, nx : c_int, c_ptrTo(arr2_in), c_ptrTo(arr2_out), FFTW_ESTIMATE);
    }

    // Set up sequential 1D version
    if (rp == 64) {
      // First backward FFT is along the y dimension, for which both input and output are length ny
      extern proc fftw_plan_dft_1d(n0: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(complex(cp)), sign : c_int, flags: c_uint) : fftw_plan;
      plan_b1 = fftw_plan_dft_1d(ny : c_int, c_ptrTo(arr_ny), c_ptrTo(arr_ny), 1, FFTW_ESTIMATE);

      // Second backward FFT is along the x dimension, which takes an nx2p-vector as input and returns an nx-vector as output
      extern proc fftw_plan_dft_c2r_1d(n0 : c_int, in_arg : c_ptr(complex(cp)), out_arg : c_ptr(real(rp)), flags : c_uint) : fftw_plan;
      plan_b2 = fftw_plan_dft_c2r_1d(nx : c_int, c_ptrTo(arr_nx2p), c_ptrTo(arr_nx), FFTW_ESTIMATE);
    } else {
      // First backward FFT is along the y dimension, for which both input and output are length ny
      extern proc fftwf_plan_dft_1d(n0: c_int, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(complex(cp)), sign : c_int, flags: c_uint) : fftw_plan;
      plan_b1 = fftwf_plan_dft_1d(ny : c_int, c_ptrTo(arr_ny), c_ptrTo(arr_ny), 1, FFTW_ESTIMATE);

      // Second backward FFT is along the x dimension, which takes an nx2p-vector as input and returns an nx-vector as output
      extern proc fftwf_plan_dft_c2r_1d(n0 : c_int, in_arg : c_ptr(complex(cp)), out_arg : c_ptr(real(rp)), flags : c_uint) : fftw_plan;
      plan_b2 = fftwf_plan_dft_c2r_1d(nx : c_int, c_ptrTo(arr_nx2p), c_ptrTo(arr_nx), FFTW_ESTIMATE);
    }

}

proc execute_forward_FFTs_2D(ref in_arr: [] real(rp), ref out_arr : [] complex(cp)) {

    if (rp == 64) {
      extern proc fftw_execute_dft_r2c(p: fftw_plan, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)));
      fftw_execute_dft_r2c(plan_f, c_ptrTo(in_arr), c_ptrTo(out_arr));
    } else {
      extern proc fftwf_execute_dft_r2c(p: fftw_plan, in_arg: c_ptr(real(rp)), out_arg: c_ptr(complex(cp)));
      fftwf_execute_dft_r2c(plan_f, c_ptrTo(in_arr), c_ptrTo(out_arr));
    }
}

proc execute_forward_FFTs(ref in_arr: [] real(rp), ref out_arr : [] complex(cp)) {

  /* First forward FFT */
  if (rp == 64) {
    forall i in D_ny {
      fftw_execute_dft_r2c(plan_f1, c_ptrTo(in_arr[i,0]), c_ptrTo(tmp_f1[i,0]));
    }
  } else {
    forall i in D_ny {
      fftwf_execute_dft_r2c(plan_f1, c_ptrTo(in_arr[i,0]), c_ptrTo(tmp_f1[i,0]));
    }
  }

  /* Transpose */
    transpose_2D(tmp_f1, tmp_f1T);

  /* Second forward FFT */
  if (rp == 64) {
    forall i in D_nx2p {
      fftw_execute_dft(plan_f2, c_ptrTo(tmp_f1T[i,0]), c_ptrTo(tmp_f2T[i,0]));
    }
  } else {
    forall i in D_nx2p {
      fftwf_execute_dft(plan_f2, c_ptrTo(tmp_f1T[i,0]), c_ptrTo(tmp_f2T[i,0]));
    }
  }

  /* Transpose */
    transpose_2D(tmp_f2T, out_arr);

}

proc execute_backward_FFTs_2D(in in_arr: [] complex(cp), ref out_arr : [] real(rp)) {

    if (rp == 64) {
      extern proc fftw_execute_dft_c2r(p: fftw_plan, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(real(rp)));
      fftw_execute_dft_c2r(plan_b, c_ptrTo(in_arr), c_ptrTo(out_arr));
    } else {
      extern proc fftwf_execute_dft_c2r(p: fftw_plan, in_arg: c_ptr(complex(cp)), out_arg: c_ptr(real(rp)));
      fftwf_execute_dft_c2r(plan_b, c_ptrTo(in_arr), c_ptrTo(out_arr));
    }

}

proc execute_backward_FFTs(in in_arr: [] complex(cp), ref out_arr : [] real(rp)) {

  /* Transpose */
    transpose_2D(in_arr, tmp_b1);

  /* First backward FFT */
  if (rp == 64) {
    forall i in D_nx2p {
      fftw_execute_dft(plan_b1, c_ptrTo(tmp_b1[i,0]), c_ptrTo(tmp_b1T[i,0]));
    }
  } else {
    forall i in D_nx2p {
      fftwf_execute_dft(plan_b1, c_ptrTo(tmp_b1[i,0]), c_ptrTo(tmp_b1T[i,0]));
    }
  }

  /* Transpose */
    transpose_2D(tmp_b1T, tmp_b2T);

  /* Second backward FFT */
  if (rp == 64) {
    forall i in D_ny {
      fftw_execute_dft_c2r(plan_b2, c_ptrTo(tmp_b2T[i,0]), c_ptrTo(out_arr[i,0]));
    }
  } else {
    forall i in D_ny {
      fftwf_execute_dft_c2r(plan_b2, c_ptrTo(tmp_b2T[i,0]), c_ptrTo(out_arr[i,0]));
    }
  }

}

proc transpose_2D(ref in_arr: [?D_in] complex(cp), ref out_arr: [?D_out] complex(cp)) {

    forall (i,j) in D_in.localSubdomain() {
      out_arr[j,i] = in_arr[i,j];
    }
}

proc normalize(ref in_arr: [?dom] real(rp)) {

    var norm = nx*ny;
    forall (j,k) in dom {
      in_arr[j,k] = in_arr[j,k] / norm;
    }

}

