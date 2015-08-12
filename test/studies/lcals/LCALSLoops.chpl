module LCALSLoops {
  use LCALSDataTypes;
  use LCALSEnums;
  use LCALSStatic;
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
      when LoopKernelID.PRESSURE_CALC_ALT {
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
      when LoopKernelID.VOL3D_CALC {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
        initData(loop_data.RealArray_1D[3], 4);
      }
      when LoopKernelID.DEL_DOT_VEC_2D {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
        initData(loop_data.RealArray_1D[3], 4);
        initData(loop_data.RealArray_1D[4], 5);
      }
      when LoopKernelID.COUPLE {
        initData(loop_data.ComplexArray_1D[0], 1);
        initData(loop_data.ComplexArray_1D[1], 2);
        initData(loop_data.ComplexArray_1D[2], 3);
        initData(loop_data.ComplexArray_1D[3], 4);
        initData(loop_data.ComplexArray_1D[4], 5);
      }
      when LoopKernelID.FIR {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
      }
      when LoopKernelID.INIT3 {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
        initData(loop_data.RealArray_1D[3], 4);
        initData(loop_data.RealArray_1D[4], 5);
      }
      when LoopKernelID.MULADDSUB {
        initData(loop_data.RealArray_1D[0], 1);
        initData(loop_data.RealArray_1D[1], 2);
        initData(loop_data.RealArray_1D[2], 3);
        initData(loop_data.RealArray_1D[3], 4);
        initData(loop_data.RealArray_1D[4], 5);
      }
      when LoopKernelID.IF_QUAD {
         initData(loop_data.RealArray_1D[0], 1);
         initData(loop_data.RealArray_1D[1], 2);
         initData(loop_data.RealArray_1D[2], 3);
         initData(loop_data.RealArray_1D[3], 4);
         initData(loop_data.RealArray_1D[4], 5);
      }
      when LoopKernelID.TRAP_INT {
        initData(loop_data.IndxArray_1D[0], 1);
        initData(loop_data.RealArray_scalars, 1);
      }
      when LoopKernelID.HYDRO_1D {
      }
      when LoopKernelID.ICCG {
      }
      when LoopKernelID.INNER_PROD {
      }
      when LoopKernelID.BAND_LIN_EQ {
      }
      when LoopKernelID.TRIDIAG_ELIM {
      }
      when LoopKernelID.EOS {
      }
      when LoopKernelID.ADI {
      }
      when LoopKernelID.INT_PREDICT {
      }
      when LoopKernelID.DIFF_PREDICT {
      }
      when LoopKernelID.FIRST_SUM {
      }
      when LoopKernelID.FIRST_DIFF {
      }
      when LoopKernelID.PIC_2D {
      }
      when LoopKernelID.PIC_1D {
      }
      when LoopKernelID.HYDRO_2D {
      }
      when LoopKernelID.GEN_LIN_RECUR {
      }
      when LoopKernelID.DISC_ORD {
      }
      when LoopKernelID.MAT_X_MAT {
      }
      when LoopKernelID.PLANCKIAN {
      }
      when LoopKernelID.IMP_HYDRO_2D {
      }
      when LoopKernelID.FIND_FIRST_MIN {
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
  extern proc printf(fmt: c_string, args...);
  extern proc printf(fmt: c_string);
  config var printCnt = 0;
  var numprinted = 0;
  proc updateChksum(stat: LoopStat, ilength: LoopLength, ra: [] real, scale_factor: real = 1.0) {
    var data => ra;
    var len = ra.numElements;
    var tchk = stat.loop_chksum[ilength];
    if printCnt > 0 {
      for i in 1..20 do
        printf("%ld", numprinted%10);
      printf("\n%f, %ld\n", tchk, len);
    }
    for (j, dat) in zip(0..#len, data) {
      tchk += (j+1)*dat*scale_factor;
    }
    if printCnt > 0 {
      printf("%f\n", tchk);
      for i in 1..20 do
        printf("%ld", numprinted%10);
      printf("\n");
      printCnt -= 1;
      numprinted += 1;
    }
    stat.loop_chksum[ilength] = tchk;
  }

  proc updateChksum(stat: LoopStat, ilength: LoopLength, val: real) {
    stat.loop_chksum[ilength] += val;
  }
  var cplxCnt = 0;
  proc updateChksum(stat: LoopStat, ilength: LoopLength, ca: [] complex, scale_factor: real = 1.0) {
    var data => ca;
    var len = ca.numElements;
    var tchk = stat.loop_chksum[ilength];
    if printCnt > 0 {
      for i in 1..20 {
        printf("%ld", cplxCnt % 10);
      }
      printf("\nComplex\n");
      printf("%f, %ld\n", tchk, len);
    }
    for (j,dat) in zip(0..#len,data) {
      tchk += (j+1)*(dat.re + dat.im)*scale_factor;
    }
    if printCnt > 0 {
      printf("%f\n", tchk);
      for i in 1..20 {
        printf("%ld", cplxCnt % 10);
      }
      printf("\n");
      cplxCnt =+ 1;
    }
    stat.loop_chksum[ilength] = tchk;
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
    loop_data.RealArray_scalars[0] = (val + 0.00123) / (val - 0.00123);

    copyTimer(lstat, ilen, ltimer);
  }
}
