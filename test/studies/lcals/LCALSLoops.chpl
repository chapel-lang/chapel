module LCALSLoops {
  use LCALSDataTypes;
  use LCALSEnums;
  use LCALSStatic;
  use LCALSDataTypes;
  use LCALSParams;
  use Timer;

  proc flushCache() {
    for i in 0..#s_loop_suite_run_info.cache_flush_data_len {
      s_loop_suite_run_info.cache_flush_data_sum += s_loop_suite_run_info.cache_flush_data[i];
    }
    s_loop_suite_run_info.cache_flush_data_sum /=
      s_loop_suite_run_info.cache_flush_data_len;
  }

  proc initData(ca: [] complex, id: int) {
    const factor = if id % 2 != 0 then 0.1+0.2i else 0.2+0.3i;

    for (a,j) in zip(ca, 0..) {
      a = factor*(j + 1.1)/(j + 1.12345);
    }
  }

  proc initData(ia: [] int, id: int) {
    ia = 0;
  }

  proc initData(ra: [] real, id: int) {
    const factor: Real_type = if id % 2 != 0 then 0.1 else 0.2;
    for (r,j) in zip(ra, 0..) {
      r = factor*(j + 1.1)/(j + 1.12345);
    }
  }

  proc loopInit(iloop:int, stat: LoopStat) {
    var loop_data = getLoopData();
    flushCache();
    stat.loop_is_run = true;
    select iloop:LoopKernelID {
      when LoopKernelID.REF_LOOP {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
      }
      when LoopKernelID.PRESSURE_CALC {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
        initData(loop_data.RealArray_1D[3], 4);
        initData(loop_data.RealArray_1D[4], 5);

        initData(loop_data.RealArray_scalars, 1);
      }
      when LoopKernelID.ENERGY_CALC {
         initData(loop_data.RealArray_1D[0], 1);
         initData(loop_data.RealArray_1D[1], 2);
         initData(loop_data.RealArray_1D[2], 3);
         initData(loop_data.RealArray_1D[3], 4);
         initData(loop_data.RealArray_1D[4], 5);
         initData(loop_data.RealArray_1D[5], 6);
         initData(loop_data.RealArray_1D[6], 7);
         initData(loop_data.RealArray_1D[7], 8);
         initData(loop_data.RealArray_1D[8], 9);
         initData(loop_data.RealArray_1D[9], 10);
         initData(loop_data.RealArray_1D[10], 11);
         initData(loop_data.RealArray_1D[11], 12);
         initData(loop_data.RealArray_1D[12], 13);
         initData(loop_data.RealArray_1D[13], 14);
         initData(loop_data.RealArray_1D[14], 15);

         initData(loop_data.RealArray_scalars, 1);
      }
      otherwise {
        halt("Unhandled loop type: ", (iloop:LoopKernelID):string);
      }
    }
  }

  proc initChksum(stat: LoopStat, ilength: LoopLength) {
    stat.loop_chksum[ilength] = 0.0;
  }

  proc loopFinalize(iloop: int, stat: LoopStat, ilength: LoopLength) {
    initChksum(stat, ilength);
    var loop_data = getLoopData();
    select iloop {
      when LoopKernelID.REF_LOOP {
        // nothing in this case
      }
      when LoopKernelID.PRESSURE_CALC {
        updateChksum(stat, ilength, loop_data.RealArray_1D[2]);
      }
      when LoopKernelID.PRESSURE_CALC_ALT {
        updateChksum(stat, ilength, loop_data.RealArray_1D[2]);
      }
      when LoopKernelID.ENERGY_CALC {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[5]);
      }
      when LoopKernelID.ENERGY_CALC_ALT {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[5]);
      }
      when LoopKernelID.VOL3D_CALC {
        updateChksum(stat, ilength, loop_data.RealArray_1D[3]);
      }
      when LoopKernelID.DEL_DOT_VEC_2D {
        updateChksum(stat, ilength, loop_data.RealArray_1D[4]);
      }
      when LoopKernelID.COUPLE {
        updateChksum(stat, ilength, loop_data.ComplexArray_1D[0]);
        updateChksum(stat, ilength, loop_data.ComplexArray_1D[1]);
        updateChksum(stat, ilength, loop_data.ComplexArray_1D[2]);
      }
      when LoopKernelID.FIR {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.INIT3 {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[1]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[2]);
      }
      when LoopKernelID.MULADDSUB {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[1]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[2]);
      }
      when LoopKernelID.IF_QUAD {
        updateChksum(stat, ilength, loop_data.RealArray_1D[3]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[4]);
      }
      when LoopKernelID.TRAP_INT {
        updateChksum(stat, ilength, loop_data.scalar_Real[0]);
      }
      when LoopKernelID.HYDRO_1D {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.ICCG {
       updateChksum(stat, ilength, loop_data.RealArray_1D_Nx4[0]);
      }
      when LoopKernelID.INNER_PROD {
        updateChksum(stat, ilength, loop_data.scalar_Real[0]);
      }
      when LoopKernelID.BAND_LIN_EQ {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.TRIDIAG_ELIM {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.EOS {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.ADI {
        updateChksum(stat, ilength, loop_data.RealArray_3D_2xNx4[0]);
        updateChksum(stat, ilength, loop_data.RealArray_3D_2xNx4[1]);
        updateChksum(stat, ilength, loop_data.RealArray_3D_2xNx4[2]);
      }
      when LoopKernelID.INT_PREDICT {
        updateChksum(stat, ilength, loop_data.RealArray_2D_Nx25[0]);
      }
      when LoopKernelID.DIFF_PREDICT {
        updateChksum(stat, ilength, loop_data.RealArray_2D_Nx25[0]);
      }
      when LoopKernelID.FIRST_SUM {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.FIRST_DIFF {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.PIC_2D {
        updateChksum(stat, ilength, loop_data.RealArray_2D_Nx25[0]);
        updateChksum(stat, ilength, loop_data.RealArray_2D_64x64[0]);
      }
      when LoopKernelID.PIC_1D {
        updateChksum(stat, ilength, loop_data.RealArray_1D[6]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[1]);
        updateChksum(stat, ilength, loop_data.RealArray_1D[7]);
      }
      when LoopKernelID.HYDRO_2D {
        updateChksum(stat, ilength, loop_data.RealArray_2D_7xN[9]);
        updateChksum(stat, ilength, loop_data.RealArray_2D_7xN[10]);
      }
      when LoopKernelID.GEN_LIN_RECUR {
        updateChksum(stat, ilength, loop_data.RealArray_1D[0]);
      }
      when LoopKernelID.DISC_ORD {
        updateChksum(stat, ilength, loop_data.RealArray_1D[7]);
      }
      when LoopKernelID.MAT_X_MAT {
        updateChksum(stat, ilength, loop_data.RealArray_2D_Nx25[0]);
      }
      when LoopKernelID.PLANCKIAN {
        updateChksum(stat, ilength, loop_data.RealArray_1D[4]);
      }
      when LoopKernelID.IMP_HYDRO_2D {
        updateChksum(stat, ilength, loop_data.RealArray_2D_7xN[0]);
      }
      when LoopKernelID.FIND_FIRST_MIN {
        updateChksum(stat, ilength, loop_data.scalar_Real[0]);
      }
      otherwise {
        halt("Unknown loop id = ", iloop);
      }
    }
  }
  proc updateChksum(stat: LoopStat, ilength: LoopLength, ra: [] real, scale_factor: real = 1.0) {
    var data => ra;
    var len = ra.numElements;
    var tchk = stat.loop_chksum[ilength];
    for (j,dat) in zip(0..#len,data) {
      tchk += (j+1)*dat*scale_factor;
    }
    stat.loop_chksum[ilength] = tchk;
  }
  proc updateChksum(stat: LoopStat, ilength: LoopLength, val: real) {
    stat.loop_chksum[ilength] += val;
  }
  proc updateChksum(stat: LoopStat, ilength: LoopLength, ca: [] complex, scale_factor: real = 1.0) {
    var data => ca;
    var len = ca.numElements;
    var tchk = stat.loop_chksum[ilength];
    for (j,dat) in zip(0..#len,data) {
      tchk += (j+1)*(dat.re + dat.im)*scale_factor;
    }
    stat.loop_chksum[ilength] = tchk;
  }

  proc runARawLoops(loop_stats: vector(LoopStat), run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    var loop_data = getLoopData();

    for iloop in 0..#loop_suite_run_info.num_loops {
      if run_loop[iloop] {
        var stat = loop_stats[iloop];
        var len = stat.loop_length[ilength];
        var num_samples = stat.samples_per_pass[ilength];
        var ltimer = new LoopTimer();
        select iloop {
          when LoopKernelID.PRESSURE_CALC {
            loopInit(iloop, stat);
            var compression => loop_data.RealArray_1D[0];
            var bvc => loop_data.RealArray_1D[1];
            var p_new => loop_data.RealArray_1D[2];
            var e_old => loop_data.RealArray_1D[3];
            var vnewc => loop_data.RealArray_1D[4];

            const cls = loop_data.scalar_Real[0];
            const p_cut = loop_data.scalar_Real[1];
            const pmin = loop_data.scalar_Real[2];
            const eosvmax = loop_data.scalar_Real[3];
            ltimer.start();
            for isamp in 0..#num_samples {
              for i in 0..#len {
                bvc[i] = cls * (compression[i] + 1.0);
              }
              for i in 0..#len {
                p_new[i] = bvc[i] * e_old[i];
                if ( abs(p_new[i]) <  p_cut ) then p_new[i] = 0.0;
                if ( vnewc[i] >= eosvmax ) then p_new[i] = 0.0;
                if ( p_new[i]  <  pmin ) then p_new[i] = pmin;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.ENERGY_CALC {
            loopInit(iloop, stat);
            var e_new = loop_data.RealArray_1D[0];
            var e_old = loop_data.RealArray_1D[1];
            var delvc = loop_data.RealArray_1D[2];
            var p_new = loop_data.RealArray_1D[3];
            var p_old = loop_data.RealArray_1D[4];
            var q_new = loop_data.RealArray_1D[5];
            var q_old = loop_data.RealArray_1D[6];
            var work = loop_data.RealArray_1D[7];
            var compHalfStep = loop_data.RealArray_1D[8];
            var pHalfStep = loop_data.RealArray_1D[9];
            var bvc = loop_data.RealArray_1D[10];
            var pbvc = loop_data.RealArray_1D[11];
            var ql_old = loop_data.RealArray_1D[12];
            var qq_old = loop_data.RealArray_1D[13];
            var vnewc = loop_data.RealArray_1D[14];

            const rho0 = loop_data.scalar_Real[0];
            const e_cut = loop_data.scalar_Real[1];
            const emin = loop_data.scalar_Real[2];
            const q_cut = loop_data.scalar_Real[3];

            ltimer.start();
            for isamp in 0..#num_samples {
              for i in 0..#len {
                e_new[i] = e_old[i] - 0.5 * delvc[i] *
                           (p_old[i] + q_old[i]) + 0.5 * work[i];
              }
              for i in 0..#len {
                if delvc[i] > 0.0 {
                  q_new[i] = 0.0;
                } else {
                  var vhalf = 1.0 / (1.0 + compHalfStep[i]);
                  var ssc = (pbvc[i] * e_new[i]
                    + vhalf * vhalf * bvc[i] * pHalfStep[i]) / rho0;
                  if ssc <= 0.1111111e-36 {
                    ssc = 0.3333333e-18;
                  } else {
                    ssc = ssc ** 0.5;
                  }
                  q_new[i] = ssc*ql_old[i] + qq_old[i];
                }
              }
              for i in 0..#len {
                e_new[i] = e_new[i] + 0.5 * delvc[i] * (3.0*(p_old[i] + q_old[i]) - 4.0*(pHalfStep[i] + q_new[i]));
              }
              for i in 0..#len {
                e_new[i] += 0.5 * work[i];
                if abs(e_new[i]) < e_cut then e_new[i] = 0.0;
                if e_new[i] < emin then e_new[i] = emin;
              }
              for i in 0..#len {
                var q_tilde = 0.0;
                if delvc[i] > 0.0 {
                  q_tilde = 0;
                } else {
                  var ssc = (pbvc[i] * e_new[i]
                           + vnewc[i] * vnewc[i] * bvc[i] * p_new[i]) / rho0;
                  if ssc <= 0.1111111e-36 then ssc = 0.3333333e-18;
                                          else ssc = ssc ** 0.5;
                  q_tilde = (ssc*ql_old[i] + qq_old[i]);
                }
                e_new[i] = e_new[i] - (7.0*(p_old[i] + q_old[i])
                                       - 8.0*(pHalfStep[i] + q_new[i])
                                       + (p_new[i] + q_tilde)) * delvc[i] / 6.0;
                if abs(e_new[i]) < e_cut then e_new[i] = 0.0;
                if e_new[i] < emin then e_new[i] = emin;
              }
              for i in 0..#len {
                if delvc[i] <= 0.0 {
                  var ssc = (pbvc[i] * e_new[i]
                             + vnewc[i] * vnewc[i] * bvc[i] * p_new[i]) / rho0;
                  if ssc <= 0.1111111e-36 then ssc = 0.3333333e-18;
                                          else ssc = ssc ** 0.5;
                  q_new[i] = (ssc*ql_old[i] + qq_old[i]) ;
                  if abs(q_new[i]) < q_cut then q_new[i] = 0.0;
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          otherwise {
          }
        }
        copyTimer(stat, ilength, ltimer);
      }
    }
  }


  proc runReferenceLoop0(lstat: LoopStat, ilen: int) {
    var loop_data = getLoopData();
    var len: int = lstat.loop_length[ilen];
    var num_samples = lstat.samples_per_pass[ilen];
    var ltimer = new LoopTimer();

    loopInit(LoopKernelID.REF_LOOP, lstat);

    var A => loop_data.RealArray_1D[0];
    var B => loop_data.RealArray_1D[1];
    var C => loop_data.RealArray_1D[2];
    ltimer.start();
    for isamp in 0..#num_samples {
      for i in 0..#len {
        C[i] = A[i] * B[i];
      }
    }
    ltimer.stop();
    copyTimer(lstat, ilen, ltimer);
  }

  proc runReferenceLoop1(lstat: LoopStat, ilen: int) {
    var loop_data = getLoopData();

    var len = lstat.loop_length[ilen];
    var num_samples = lstat.samples_per_pass[ilen];
    var ltimer = new LoopTimer();

    loopInit(LoopKernelID.REF_LOOP, lstat);

    var a => loop_data.RealArray_1D[0];
    var b => loop_data.RealArray_1D[1];
    var c => loop_data.RealArray_1D[2];

    var val = 0.0;

    ltimer.start();
    for isamp in 0..#num_samples {

      var q = 0.0; // q is never changed - val is always 0.0.
      for i in 0..#len {
        c[i] = a[i] * b[i];
      }

      val = q*isamp;
    }
    ltimer.stop();

    //
    // RDH added this. Without it compiler may optimize out
    // outer sampling loop because value of q was not used.
    //
    loop_data.scalar_Real[0] = (val + 0.00123) / (val - 0.00123);

    copyTimer(lstat, ilen, ltimer);
  }
}
