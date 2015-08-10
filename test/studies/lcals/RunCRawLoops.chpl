module RunCRawLoops {
  use LCALSDataTypes;
  use Timer;

  proc runCRawLoops(loop_stats: vector(LoopStat), run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    var loop_data = getLoopData();
    for iloop in 0..#loop_suite_run_info.num_loops {
      if run_loop[iloop] {
        var stat = loop_stats[iloop];
        var len = stat.loop_length[ilength];
        var num_samples = stat.samples_per_pass[ilength];
        var ltimer = new LoopTimer();
        if printLoopKernelIDs then
          writeln(iloop: LoopKernelID);
        select iloop {
          when LoopKernelID.HYDRO_1D {
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1],
                z => loop_data.RealArray_1D[2];

            const q = loop_data.scalar_Real[0],
                  r = loop_data.scalar_Real[1],
                  t = loop_data.scalar_Real[2];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                x[k] = q + y[k]*(r*z[k+10] + t*z[k+11]);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.ICCG {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                v => loop_data.RealArray_1D[1];
            var ii, ipnt, ipntp, i: int;
            ltimer.start();
            for isamp in 0..#num_samples {
              ii = len;
              ipntp = 0;
              do {
                ipnt = ipntp;
                ipntp += ii;
                ii /= 2;
                i = ipntp;
                for k in ipnt+1..ipntp-1 by 2 {
                  x[i] = x[k] - v[k]*x[k-1] - v[k+1]*x[k+1];
                }
              } while (ii > 0);
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.INNER_PROD {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                z => loop_data.RealArray_1D[1];
            var q = 0.0, val = 0.0;
            ltimer.start();
            for isamp in 0..#num_samples {
              q = 0.0;
              for k in 0..#len {
                q = z[k]*x[k];
              }
              val = q*isamp;
            }
            ltimer.stop();
            loop_data.scalar_Real[0] = (val+0.00123) / (val-0.00123);
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.BAND_LIN_EQ {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1];
            var lw = 0, temp = 0.0;

            ltimer.start();
            for isamp in 0..#num_samples {
              var m = (1001-7)/2;
              for k in 6..1000 by m {
                lw = k-6;
                temp = x[k-1];
                for j in 4..len-1 by 5 {
                  temp -= x[lw]*y[j];
                  lw += 1;
                }
                x[k-1] = y[4]*temp;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.TRIDIAG_ELIM {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1],
                z => loop_data.RealArray_1D[2];
            ltimer.start();
            for isamp in 0..#num_samples {
              for i in 1..len-1 {
                x[i] = z[i]*(y[i] - x[i-1]);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.EOS {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1],
                z => loop_data.RealArray_1D[2],
                u => loop_data.RealArray_1D[3];

            const q = loop_data.scalar_Real[0],
                  r = loop_data.scalar_Real[1],
                  t = loop_data.scalar_Real[2];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                x[k] = u[k] + r*(z[k] + r*y[k]) +
                       t*(u[k+3] + r*(u[k+2] + r*u[k+1]) +
                       t*(u[k+6] + q*(u[k+5] + q*u[k+4])));
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.ADI {
            loopInit(iloop, stat);
            var du1 => loop_data.RealArray_1D[0],
                du2 => loop_data.RealArray_1D[1],
                du3 => loop_data.RealArray_1D[2];
            halt("multidim cases not implemented ", iloop:LoopKernelID);

          }
          when LoopKernelID.INT_PREDICT {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
          }
          when LoopKernelID.DIFF_PREDICT {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
          }
          when LoopKernelID.FIRST_SUM {
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1];
            ltimer.start();
            for isamp in 0..#num_samples {
              x[0] = y[0];
              for k in 1..len-1 {
                x[k] = x[k-1] + y[k];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.FIRST_DIFF {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                x[k] = y[k+1] - y[k];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.PIC_2D {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
          }
          when LoopKernelID.PIC_1D {
            loopInit(iloop, stat);
            var vx = loop_data.RealArray_1D[0],
                xx = loop_data.RealArray_1D[1],
                xi = loop_data.RealArray_1D[2],
                ex = loop_data.RealArray_1D[3],
                ex1 = loop_data.RealArray_1D[4],
                dex = loop_data.RealArray_1D[5],
                dex1 = loop_data.RealArray_1D[6],
                rh = loop_data.RealArray_1D[7],
                rx = loop_data.RealArray_1D[8];

            const flx = loop_data.scalar_Real[0];
            var ix => loop_data.IndxArray_1D[2],
                ir => loop_data.IndxArray_1D[3],
                grd => loop_data.IndxArray_1D[4];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                vx[k] = 0.0;
                xx[k] = 0.0;
                ix[k] = grd[k]:int;
                xi[k] = ix[k]:real;
                ex1[k] = ex[ix[k]-1];
                dex1[k] = dex[ix[k]-1];
              }
              for k in 0..#len {
                vx[k] = vx[k] + ex1[k] + (xx[k] - xi[k])*dex1[k];
                xx[k] = xx[k] + vx[k]  + flx;
                ir[k] = xx[k]:int;
                rx[k] = xx[k] - ir[k];
                ir[k] = (ir[k] & (2048-1)) + 1;
                xx[k] = rx[k] + ir[k];
              }
              for k in 0..#len {
                rh[ir[k]-1] += 1.0 - rx[k];
                rh[ir[k]] += rx[k];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.HYDRO_2D {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
            ltimer.start();
            for isamp in 0..#num_samples {
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.GEN_LIN_RECUR {
            loopInit(iloop, stat);
            var b5 => loop_data.RealArray_1D[0],
                sa => loop_data.RealArray_1D[1],
                sb => loop_data.RealArray_1D[2];

            var stb5 = loop_data.scalar_Real[0];
            var kb5i = 0;
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                b5[k+kb5i] = sa[k] + stb5*sb[k];
                stb5 = b5[k+kb5i] - stb5;
              }
              for i in 1..len {
                const k = len - i;
                b5[k+kb5i] = sa[k] + stb5*sb[k];
                stb5 = b5[k+kb5i] - stb5;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.DISC_ORD {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1],
                z => loop_data.RealArray_1D[2],
                u => loop_data.RealArray_1D[3],
                v => loop_data.RealArray_1D[4],
                w => loop_data.RealArray_1D[5],
                g => loop_data.RealArray_1D[6],
                xx => loop_data.RealArray_1D[7],
                vx => loop_data.RealArray_1D[9];
            const s = loop_data.scalar_Real[0],
                  t = loop_data.scalar_Real[1],
                  dk = loop_data.scalar_Real[2];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                var di = y[k] - g[k] / (xx[k] + dk);
                var dn = 0.2;
                if di != 0 {
                  dn = z[k]/di;
                  if t < dn then dn = t;
                  if s > dn then dn = s;
                }
                x[k] = ((w[k] + v[k]*dn) * xx[k] + u[k]) / (vx[k] + v[k]*dn);
                xx[k+1] = (x[k] - xx[k]) * dn + xx[k];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.MAT_X_MAT {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
            loopInit(iloop, stat);
            ltimer.start();
            for isamp in 0..#num_samples {
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.PLANCKIAN {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0],
                y => loop_data.RealArray_1D[1],
                u => loop_data.RealArray_1D[2],
                v => loop_data.RealArray_1D[3],
                w => loop_data.RealArray_1D[4];
            var expmax = 20.0;
            u[len-1] = 0.99 * expmax*v[len-1];
            ltimer.start();
            for isamp in 0..#num_samples {
              for k in 0..#len {
                y[k] = u[k] / v[k];
                w[k] = x[k] / (exp(y[k]) - 1.0);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.IMP_HYDRO_2D {
            halt("multidim cases not implemented ", iloop:LoopKernelID);
            loopInit(iloop, stat);
            ltimer.start();
            for isamp in 0..#num_samples {
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.FIND_FIRST_MIN {
            loopInit(iloop, stat);
            var x => loop_data.RealArray_1D[0];
            var m, val = 0;
            ltimer.start();
            for isamp in 0..#num_samples {
              x[len/2] = (-1.0e+10)*isamp;
              m = 0;
              for k in 1..len-1 {
                if x[k] < x[m] then m = k;
              }
              val = isamp;
            }
            ltimer.stop();
            loop_data.scalar_Real[0] = 0.123*val*m;
            loopFinalize(iloop, stat, ilength);
          }
          otherwise {
          }
        }
        copyTimer(stat, ilength, ltimer);
      }
    }
  }
}
