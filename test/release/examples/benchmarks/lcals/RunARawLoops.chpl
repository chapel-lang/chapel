module RunARawLoops {
  use LCALSDataTypes;
  use Timer;

  proc runARawLoops(loop_stats:[] shared LoopStat, run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    var loop_data = getLoopData();

    for iloop in loop_suite_run_info.loop_kernel_dom {
      if run_loop[iloop] {
        var stat = loop_stats[iloop].borrow();
        var len = stat.loop_length[ilength];
        var num_samples = stat.samples_per_pass[ilength];
        var ltimer = new owned LoopTimer();

        select iloop {
          when LoopKernelID.PRESSURE_CALC {
            loopInit(iloop, stat);

            ref compression = loop_data.RealArray_1D[0];
            ref bvc = loop_data.RealArray_1D[1];
            ref p_new = loop_data.RealArray_1D[2];
            ref e_old = loop_data.RealArray_1D[3];
            ref vnewc = loop_data.RealArray_1D[4];

            const cls = loop_data.RealArray_scalars[0];
            const p_cut = loop_data.RealArray_scalars[1];
            const pmin = loop_data.RealArray_scalars[2];
            const eosvmax = loop_data.RealArray_scalars[3];

            ltimer.start();
            for 0..#num_samples {
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
          when LoopKernelID.PRESSURE_CALC_ALT {
            stat.loop_is_run = false;
          }
          when LoopKernelID.ENERGY_CALC {
            loopInit(iloop, stat);
            ref e_new = loop_data.RealArray_1D[0];
            ref e_old = loop_data.RealArray_1D[1];
            ref delvc = loop_data.RealArray_1D[2];
            ref p_new = loop_data.RealArray_1D[3];
            ref p_old = loop_data.RealArray_1D[4];
            ref q_new = loop_data.RealArray_1D[5];
            ref q_old = loop_data.RealArray_1D[6];
            ref work = loop_data.RealArray_1D[7];
            ref compHalfStep = loop_data.RealArray_1D[8];
            ref pHalfStep = loop_data.RealArray_1D[9];
            ref bvc = loop_data.RealArray_1D[10];
            ref pbvc = loop_data.RealArray_1D[11];
            ref ql_old = loop_data.RealArray_1D[12];
            ref qq_old = loop_data.RealArray_1D[13];
            ref vnewc = loop_data.RealArray_1D[14];

            const rho0 = loop_data.RealArray_scalars[0];
            const e_cut = loop_data.RealArray_scalars[1];
            const emin = loop_data.RealArray_scalars[2];
            const q_cut = loop_data.RealArray_scalars[3];

            ltimer.start();
            for 0..#num_samples {
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
          when LoopKernelID.ENERGY_CALC_ALT {
            stat.loop_is_run = false;
          }
          when LoopKernelID.VOL3D_CALC {
            loopInit(iloop, stat);
            ref x   = loop_data.RealArray_1D[0];
            ref y   = loop_data.RealArray_1D[1];
            ref z   = loop_data.RealArray_1D[2];
            ref vol = loop_data.RealArray_1D[3];

            var dom = new ADomain(ilength, 3);

            ref x0 = x;
            ref x1 = x0[1..].reindex(0..#x0.numElements-1);
            ref x2 = x0[dom.jp..].reindex(0..#x0.numElements-dom.jp);
            ref x3 = x1[dom.jp..].reindex(0..#x1.numElements-dom.jp);
            ref x4 = x0[dom.kp..].reindex(0..#x0.numElements-dom.kp);
            ref x5 = x1[dom.kp..].reindex(0..#x1.numElements-dom.kp);
            ref x6 = x2[dom.kp..].reindex(0..#x2.numElements-dom.kp);
            ref x7 = x3[dom.kp..].reindex(0..#x3.numElements-dom.kp);

            ref y0 = y;
            ref y1 = y0[1..].reindex(0..#y0.numElements-1);
            ref y2 = y0[dom.jp..].reindex(0..#y0.numElements-dom.jp);
            ref y3 = y1[dom.jp..].reindex(0..#y1.numElements-dom.jp);
            ref y4 = y0[dom.kp..].reindex(0..#y0.numElements-dom.kp);
            ref y5 = y1[dom.kp..].reindex(0..#y1.numElements-dom.kp);
            ref y6 = y2[dom.kp..].reindex(0..#y2.numElements-dom.kp);
            ref y7 = y3[dom.kp..].reindex(0..#y3.numElements-dom.kp);

            ref z0 = z;
            ref z1 = z0[1..].reindex(0..#z0.numElements-1);
            ref z2 = z0[dom.jp..].reindex(0..#z0.numElements-dom.jp);
            ref z3 = z1[dom.jp..].reindex(0..#z1.numElements-dom.jp);
            ref z4 = z0[dom.kp..].reindex(0..#z0.numElements-dom.kp);
            ref z5 = z1[dom.kp..].reindex(0..#z1.numElements-dom.kp);
            ref z6 = z2[dom.kp..].reindex(0..#z2.numElements-dom.kp);
            ref z7 = z3[dom.kp..].reindex(0..#z3.numElements-dom.kp);

            const vnormq = 0.083333333333333333;
            ltimer.start();
            for 0..#num_samples {
              for i in dom.fpz..dom.lpz {
                const x71 = x7[i] - x1[i],
                      x72 = x7[i] - x2[i],
                      x74 = x7[i] - x4[i],
                      x30 = x3[i] - x0[i],
                      x50 = x5[i] - x0[i],
                      x60 = x6[i] - x0[i];

                const y71 = y7[i] - y1[i],
                      y72 = y7[i] - y2[i],
                      y74 = y7[i] - y4[i],
                      y30 = y3[i] - y0[i],
                      y50 = y5[i] - y0[i],
                      y60 = y6[i] - y0[i];

                const z71 = z7[i] - z1[i],
                      z72 = z7[i] - z2[i],
                      z74 = z7[i] - z4[i],
                      z30 = z3[i] - z0[i],
                      z50 = z5[i] - z0[i],
                      z60 = z6[i] - z0[i];

                var xps = x71 + x60,
                    yps = y71 + y60,
                    zps = z71 + z60;

                var cyz = y72 * z30 - z72 * y30,
                    czx = z72 * x30 - x72 * z30,
                    cxy = x72 * y30 - y72 * x30;

                vol[i] = xps * cyz + yps * czx + zps * cxy ;

                xps = x72 + x50 ;
                yps = y72 + y50 ;
                zps = z72 + z50 ;

                cyz = y74 * z60 - z74 * y60 ;
                czx = z74 * x60 - x74 * z60 ;
                cxy = x74 * y60 - y74 * x60 ;
                vol[i] += xps * cyz + yps * czx + zps * cxy ;

                xps = x74 + x30 ;
                yps = y74 + y30 ;
                zps = z74 + z30 ;

                cyz = y71 * z50 - z71 * y50 ;
                czx = z71 * x50 - x71 * z50 ;
                cxy = x71 * y50 - y71 * x50 ;
                vol[i] += xps * cyz + yps * czx + zps * cxy ;

                vol[i] *= vnormq ;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.DEL_DOT_VEC_2D {
            loopInit(iloop, stat);
            ref x    = loop_data.RealArray_1D[0];
            ref y    = loop_data.RealArray_1D[1];
            ref xdot = loop_data.RealArray_1D[2];
            ref ydot = loop_data.RealArray_1D[3];
            ref div  = loop_data.RealArray_1D[4];

            var dom = new ADomain(ilength, 2);

            ref x4 = x.reindex(0..#(x.numElements));
            ref x1 = x4[1..].reindex(0..#(x.numElements-1));
            ref x2 = x1[dom.jp..].reindex(0..#(x.numElements-1-dom.jp));
            ref x3 = x4[dom.jp..].reindex(0..#(x.numElements-dom.jp));

            ref y4 = y.reindex(0..#(y.numElements));
            ref y1 = y4[1..].reindex(0..#(y.numElements-1));
            ref y2 = y1[dom.jp..].reindex(0..#(y.numElements-1-dom.jp));
            ref y3 = y4[dom.jp..].reindex(0..#(y.numElements-dom.jp));

            ref fx4 = xdot.reindex(0..#(xdot.numElements));
            ref fx1 = fx4[1..].reindex(0..#(xdot.numElements-1));
            ref fx2 = fx1[dom.jp..].reindex(0..#(xdot.numElements-1-dom.jp));
            ref fx3 = fx4[dom.jp..].reindex(0..#(xdot.numElements-dom.jp));

            ref fy4 = ydot.reindex(0..#(ydot.numElements));
            ref fy1 = fy4[1..].reindex(0..#(ydot.numElements-1));
            ref fy2 = fy1[dom.jp..].reindex(0..#(ydot.numElements-1-dom.jp));
            ref fy3 = fy4[dom.jp..].reindex(0..#(ydot.numElements-dom.jp));

            const ptiny = 1.0e-20;
            const half = 0.5;
            ltimer.start();
            for 0..#num_samples {
              for ii in 0..#dom.n_real_zones {
                const i  = dom.real_zones[ii];

                const xi  = half * (x1[i]  + x2[i]  - x3[i]  - x4[i]);
                const xj  = half * (x2[i]  + x3[i]  - x4[i]  - x1[i]);

                const yi  = half * (y1[i]  + y2[i]  - y3[i]  - y4[i]);
                const yj  = half * (y2[i]  + y3[i]  - y4[i]  - y1[i]);

                const fxi = half * (fx1[i] + fx2[i] - fx3[i] - fx4[i]);
                const fxj = half * (fx2[i] + fx3[i] - fx4[i] - fx1[i]);

                const fyi = half * (fy1[i] + fy2[i] - fy3[i] - fy4[i]);
                const fyj = half * (fy2[i] + fy3[i] - fy4[i] - fy1[i]);

                const rarea  = 1.0 / (xi * yj - xj * yi + ptiny);

                const dfxdx  = rarea * (fxi * yj - fxj * yi);

                const dfydy  = rarea * (fyj * xi - fyi * xj);

                const affine = (fy1[i] + fy2[i] + fy3[i] + fy4[i]) /
                               (y1[i]  + y2[i]  + y3[i]  + y4[i]);

                div[i] = dfxdx + dfydy + affine;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.COUPLE {
            loopInit(iloop, stat);
            ref t0 = loop_data.ComplexArray_1D[0];
            ref t1 = loop_data.ComplexArray_1D[1];
            ref t2 = loop_data.ComplexArray_1D[2];
            ref denac = loop_data.ComplexArray_1D[3];
            ref denlw = loop_data.ComplexArray_1D[4];

            var dom = new ADomain(ilength, 3);

            var imin = dom.imin, imax = dom.imax,
                jmin = dom.jmin, jmax = dom.jmax,
                kmin = dom.kmin, kmax = dom.kmax;

            const clight = 3.0e10, csound = 3.09e7, omega0=0.9,
                  omegar = 0.9, dt = 0.208, c10 = 0.25*(clight/csound),
                  fratio = sqrt(omegar/omega0), r_fratio = 1.0/fratio,
                  c20 = c10*r_fratio;
            const ireal = 0.0 + 1.0i;
            ltimer.start();
            for 0..#num_samples {
              for k in kmin..kmax-1 {
                for j in jmin..jmax-1 {
                  var it0    = (k*(jmax+1) + j) * (imax+1);
                  var idenac = (k*(jmax+2) + j) * (imax+2);
                  for i in imin..imax-1 {
                    var c1 = c10*denac[idenac+i], c2 = c20*denlw[it0+i];

                    var c1re = c1.re, c1im = c1.im,
                        c2re = c2.re, c2im = c2.im;
                    const zlam = sqrt(c1re*c1re + c1im*c1im +
                                      c2re*c2re + c2im*c2im + 1.0e-34);
                    const snlamt = sin(zlam*dt*0.5),
                          cslamt = cos(zlam*dt*0.5);
                    const a0t = t0[it0+i],
                          a1t = t1[it0+i],
                          a2t = t2[it0+i] * fratio;
                    const r_zlam = 1.0/zlam;

                    c1 *= r_zlam;
                    c2 *= r_zlam;

                    const zac1 = c1.re*c1.re + c1.im*c1.im;
                    const zac2 = c2.re*c2.re + c2.im*c2.im;

                    // new A0
                    var z3 = (c1*a1t + c2*a2t) * snlamt;
                    t0[it0+i] = a0t*cslamt - ireal*z3;

                    // new A1
                    var r = zac1*cslamt + zac2;
                    var z5 = c2*a2t,
                        z4 = conjg(c1) * z5 * (cslamt-1);
                    z3 = conjg(c1) * a0t * snlamt;
                    t1[it0+i] = a1t*r + z4 - ireal*z3;

                    // new A2
                    r = zac1 + zac2*cslamt;
                    z5 = c1*a1t;
                    z4 = conjg(c2) * z5 * (cslamt-1);
                    z3 = conjg(c2) * a0t * snlamt;
                    t2[it0+i] = (a2t*r + z4 - ireal*z3) * r_fratio;
                  }
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.FIR {
            loopInit(iloop, stat);
            ref output = loop_data.RealArray_1D[0];
            ref input = loop_data.RealArray_1D[1];
            const coefflen = 16;
            const coeff = [3.0, -1.0, -1.0, -1.0,
                           -1.0, 3.0, -1.0, -1.0,
                           -1.0, -1.0, 3.0, -1.0,
                           -1.0, -1.0, -1.0, 3.0];
            const len_minus_coeff = len - coefflen;
            var val = 0;

            ltimer.start();
            for isamp in 0..#num_samples {
              for i in 0..#len_minus_coeff {
                var sum = 0.0;
                for j in 0..#coefflen {
                  sum += coeff[j+1]*input[i+j];
                }
                output[i] = sum;
              }
              val = isamp;
            }
            ltimer.stop();
            loop_data.RealArray_scalars[0] = (val + 0.00123) / (val - 0.00123);
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
