module RunBRawLoops {
  use LCALSDataTypes;
  use LCALSEnums;
  use LCALSStatic;
  use Timer;

  proc trap_int_func(x: real, y: real, xp: real, yp: real) {
    var denom = let xs = (x - xp), ys = (y - yp) in xs*xs + ys*ys;
    return 1.0 / sqrt(denom);
  }

  proc runBRawLoops(loop_stats:[] shared LoopStat, run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    var loop_data = getLoopData();
    for iloop in loop_suite_run_info.loop_kernel_dom {
      if run_loop[iloop] {
        var stat = loop_stats[iloop].borrow();
        var len = stat.loop_length[ilength];
        var num_samples = stat.samples_per_pass[ilength];
        var ltimer = new LoopTimer();

        select iloop {
          when LoopKernelID.INIT3 {
            loopInit(iloop, stat);
            ref out1 = loop_data.RealArray_1D[0];
            ref out2 = loop_data.RealArray_1D[1];
            ref out3 = loop_data.RealArray_1D[2];
            ref in1  = loop_data.RealArray_1D[3];
            ref in2  = loop_data.RealArray_1D[4];
            ltimer.start();
            for 0..#num_samples {
              for i in 0..#len {
                const res = -in1[i] - in2[i];
                out3[i] = res;
                out2[i] = res;
                out1[i] = res;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.MULADDSUB {
            loopInit(iloop, stat);
            ref out1 = loop_data.RealArray_1D[0];
            ref out2 = loop_data.RealArray_1D[1];
            ref out3 = loop_data.RealArray_1D[2];
            ref in1  = loop_data.RealArray_1D[3];
            ref in2  = loop_data.RealArray_1D[4];
            ltimer.start();
            for 0..#num_samples {
              for i in 0..#len {
                out1[i] = in1[i] * in2[i];
                out2[i] = in1[i] + in2[i];
                out3[i] = in1[i] - in2[i];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.IF_QUAD {
            loopInit(iloop, stat);
            ref a  = loop_data.RealArray_1D[0];
            ref b  = loop_data.RealArray_1D[1];
            ref c  = loop_data.RealArray_1D[2];
            ref x1 = loop_data.RealArray_1D[3];
            ref x2 = loop_data.RealArray_1D[4];
            ltimer.start();
            for 0..#num_samples {
              for i in 0..#len {
                var s = b[i]*b[i] - 4.0*a[i]*c[i];
                if s >= 0 {
                  s = sqrt(s);
                  x2[i] = (-b[i]+s) / (2.0*a[i]);
                  x1[i] = (-b[i]-s) / (2.0*a[i]);
                } else {
                  x2[i] = 0.0;
                  x1[i] = 0.0;
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.TRAP_INT {
            loopInit(iloop, stat);
            var xn = loop_data.RealArray_scalars[0];
            var x0 = loop_data.RealArray_scalars[1];
            var xp = loop_data.RealArray_scalars[2];
            var y  = loop_data.RealArray_scalars[3];
            var yp = loop_data.RealArray_scalars[4];

            var nx =  loop_data.IndxArray_1D[0][0] + 1;

            const h = (xn - x0) / nx;
            var sumx = 0.5 * (trap_int_func(x0, y, xp, yp) +
                              trap_int_func(xn, y, xp, yp));
            var val = 0.0;
            ltimer.start();
            for 0..#num_samples {
              for i in 0..(len-1):int(32) {
                var x = x0 + i*h;
                sumx += trap_int_func(x, y, xp, yp);
              }
              val = sumx * h;
            }
            ltimer.stop();
            loop_data.RealArray_scalars[0] = (val + 0.00123) / (val - 0.00123);
            loopFinalize(iloop, stat, ilength);
          }
          otherwise {
            //writeln("unknown loop id: ", iloop, " LoopKernelID: ", iloop:LoopKernelID);
          }
        }
        copyTimer(stat, ilength, ltimer);
      }
    }
  }
}
