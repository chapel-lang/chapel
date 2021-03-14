module RunCRawLoops {
  use LCALSDataTypes;
  use Timer;

  proc runCRawLoops(loop_stats:[] shared LoopStat, run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    var loop_data = getLoopData();
    for iloop in loop_suite_run_info.loop_kernel_dom {
      if run_loop[iloop] {
        var stat = loop_stats[iloop].borrow();
        var len = stat.loop_length[ilength];
        var num_samples = stat.samples_per_pass[ilength];
        var ltimer = new LoopTimer();

        select iloop {
          when LoopKernelID.HYDRO_1D {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1],
                z = loop_data.RealArray_1D[2];

            const q = loop_data.RealArray_scalars[0],
                  r = loop_data.RealArray_scalars[1],
                  t = loop_data.RealArray_scalars[2];
            ltimer.start();
            for 0..#num_samples {
              for k in 0..#len {
                x[k] = q + y[k]*(r*z[k+10] + t*z[k+11]);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.ICCG {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D_Nx4[0],
                v = loop_data.RealArray_1D_Nx4[1];
            var ii, ipnt, ipntp, i: int;
            ltimer.start();
            for 0..#num_samples {
              ii = len;
              ipntp = 0;
              do {
                ipnt = ipntp;
                ipntp += ii;
                ii /= 2;
                i = ipntp;
                for k in ipnt+1..ipntp-1 by 2 {
                  i += 1;
                  x[i] = x[k] - v[k]*x[k-1] - v[k+1]*x[k+1];
                }
              } while (ii > 0);
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.INNER_PROD {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                z = loop_data.RealArray_1D[1];
            var q = 0.0, val = 0.0;
            ltimer.start();
            for isamp in 0..#num_samples {
              q = 0.0;
              for k in 0..#len {
                q += z[k] * x[k];
              }
              val = q*isamp;
            }
            ltimer.stop();
            loop_data.RealArray_scalars[0] = (val+0.00123) / (val-0.00123);
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.BAND_LIN_EQ {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1];
            var lw = 0, temp = 0.0;

            ltimer.start();
            for 0..#num_samples {
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
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1],
                z = loop_data.RealArray_1D[2];
            ltimer.start();
            for 0..#num_samples {
              for i in 1..len-1 {
                x[i] = z[i]*(y[i] - x[i-1]);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.EOS {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1],
                z = loop_data.RealArray_1D[2],
                u = loop_data.RealArray_1D[3];

            const q = loop_data.RealArray_scalars[0],
                  r = loop_data.RealArray_scalars[1],
                  t = loop_data.RealArray_scalars[2];
            ltimer.start();
            for 0..#num_samples {
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
            ref du1 = loop_data.RealArray_1D[0],
                du2 = loop_data.RealArray_1D[1],
                du3 = loop_data.RealArray_1D[2];

            var u1 = loop_data.RealArray_3D_2xNx4[0].borrow(),
                u2 = loop_data.RealArray_3D_2xNx4[1].borrow(),
                u3 = loop_data.RealArray_3D_2xNx4[2].borrow();

            const sig = loop_data.RealArray_scalars[0],
                  a11 = loop_data.RealArray_scalars[1],
                  a12 = loop_data.RealArray_scalars[2],
                  a13 = loop_data.RealArray_scalars[3],
                  a21 = loop_data.RealArray_scalars[4],
                  a22 = loop_data.RealArray_scalars[5],
                  a23 = loop_data.RealArray_scalars[6],
                  a31 = loop_data.RealArray_scalars[7],
                  a32 = loop_data.RealArray_scalars[8],
                  a33 = loop_data.RealArray_scalars[9];

            const nl1 = 0, nl2 = 1;

            ltimer.start();
            for 0..#num_samples {
              for kx in 1..3-1 {
                for ky in 1..len-1 {
                  du1[ky] = u1[nl1, ky+1, kx] - u1[nl1, ky-1, kx];
                  du2[ky] = u2[nl1, ky+1, kx] - u2[nl1, ky-1, kx];
                  du3[ky] = u3[nl1, ky+1, kx] - u3[nl1, ky-1, kx];

                  u1[nl2, ky, kx] =
                    u1[nl1, ky, kx] + a11*du1[ky] + a12*du2[ky] + a13*du3[ky] +
                    sig*(u1[nl1, ky, kx+1] - 2.0*u1[nl1, ky, kx] + u1[nl1, ky, kx-1]);
                  u2[nl2, ky, kx] =
                     u2[nl1, ky, kx] + a21*du1[ky] + a22*du2[ky] + a23*du3[ky] +
                     sig*(u2[nl1, ky, kx+1]-2.0*u2[nl1, ky, kx]+u2[nl1, ky, kx-1]);
                  u3[nl2, ky, kx] =
                     u3[nl1, ky, kx] + a31*du1[ky] + a32*du2[ky] + a33*du3[ky] +
                     sig*(u3[nl1, ky, kx+1]-2.0*u3[nl1, ky, kx]+u3[nl1, ky, kx-1]);
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.INT_PREDICT {
            loopInit(iloop, stat);
            ref px = loop_data.RealArray_2D_Nx25[0];

            const dm22 = loop_data.RealArray_scalars[0],
                  dm23 = loop_data.RealArray_scalars[1],
                  dm24 = loop_data.RealArray_scalars[2],
                  dm25 = loop_data.RealArray_scalars[3],
                  dm26 = loop_data.RealArray_scalars[4],
                  dm27 = loop_data.RealArray_scalars[5],
                  dm28 = loop_data.RealArray_scalars[6],
                  c0 = loop_data.RealArray_scalars[7];
            ltimer.start();
            for 0..#num_samples {
              for i in 0..#len {
                px[i,0] = dm28*px[i,12] + dm27*px[i,11] + dm26*px[i,10] +
                          dm25*px[i,9]  + dm24*px[i,8]  + dm23*px[i,7]  +
                          dm22*px[i,6]  + c0*(px[i,4] + px[i,5]) + px[i,2];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.DIFF_PREDICT {
            loopInit(iloop, stat);
            ref px = loop_data.RealArray_2D_Nx25[0];
            ref cx = loop_data.RealArray_2D_Nx25[1];
            ltimer.start();
            for 0..#num_samples {
              for i in 0..#len {
                var ar, br, cr: real;
                ar       =      cx[i,4];
                br       = ar - px[i,4];
                px[i,4]  = ar;
                cr       = br - px[i,5];
                px[i,5]  = br;
                ar       = cr - px[i,6];
                px[i,6]  = cr;
                br       = ar - px[i,7];
                px[i,7]  = ar;
                cr       = br - px[i,8];
                px[i,8]  = br;
                ar       = cr - px[i,9];
                px[i,9]  = cr;
                br       = ar - px[i,10];
                px[i,10] = ar;
                cr       = br - px[i,11];
                px[i,11] = br;
                px[i,13] = cr - px[i,12];
                px[i,12] = cr;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.FIRST_SUM {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1];
            ltimer.start();
            for 0..#num_samples {
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
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1];
            ltimer.start();
            for 0..#num_samples {
              for k in 0..#len {
                x[k] = y[k+1] - y[k];
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.PIC_2D {
            loopInit(iloop, stat);
            ref p = loop_data.RealArray_2D_Nx25[0],
                b = loop_data.RealArray_2D_Nx25[1],
                c = loop_data.RealArray_2D_Nx25[2];

            ref y = loop_data.RealArray_1D[0],
                z = loop_data.RealArray_1D[1];

            ref e = loop_data.IndxArray_1D[0],
                f = loop_data.IndxArray_1D[1];

            ref h = loop_data.RealArray_2D_64x64[0];
            ltimer.start();
            proc overIndexMapper(i,j) {
              /* The reference version of this kernel is over-indexing a
                 logical Nx25 array using indices like (16,26).  With bounds
                 checking enabled, 26 is out of bounds in the second dimension.
                 Convert the over-indexed pairs into in-bounds pairs */
              return (i+j/25, j%25);
            }
            for 0..#num_samples {
              for ip in 0..#len {
                var i1, j1, i2, j2: int;
                // These casts to int(32) overflow and behave differently
                // than if they were cast to default sized int (int(64)).
                // Cast to int(32) to maintain the behavior of the C
                // reference version.
                i1 = p[ip,0]: int(32);
                j1 = p[ip,1]: int(32);
                i1 &= 64-1;
                j1 &= 64-1;
                p[ip,2] += b[overIndexMapper(j1,i1)];
                p[ip,3] += c[overIndexMapper(j1,i1)];
                p[ip,0] += p[ip,2];
                p[ip,1] += p[ip,3];
                i2 = p[ip,0]: int(32);
                j2 = p[ip,1]: int(32);
                i2 &= 64-1;
                j2 &= 64-1;
                p[ip,0] += y[i2+32];
                p[ip,1] += z[j2+32];
                i2 += e[i2+32];
                j2 += f[j2+32];
                h[j2,i2] += 1.0;
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.PIC_1D {
            loopInit(iloop, stat);
            ref vx = loop_data.RealArray_1D[0],
                xx = loop_data.RealArray_1D[1],
                xi = loop_data.RealArray_1D[2],
                ex = loop_data.RealArray_1D[3],
                ex1 = loop_data.RealArray_1D[4],
                dex = loop_data.RealArray_1D[5],
                dex1 = loop_data.RealArray_1D[6],
                rh = loop_data.RealArray_1D[7],
                rx = loop_data.RealArray_1D[8];

            const flx = loop_data.RealArray_scalars[0];
            ref ix = loop_data.IndxArray_1D[2],
                ir = loop_data.IndxArray_1D[3],
                grd = loop_data.IndxArray_1D[4];
            ltimer.start();
            for 0..#num_samples {
              for k in 0..#len {
                vx[k] = 0.0;
                xx[k] = 0.0;
                ix[k] = grd[k]:int;
                xi[k] = ix[k]:real;
                // ix[k] == 0, so the accesses of ex[ix[k]-1] and dex[ix[k]-1]
                // are out of bounds. This also happens in the reference version.
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
            loopInit(iloop, stat);
            ref za = loop_data.RealArray_2D_7xN[0],
                zb = loop_data.RealArray_2D_7xN[1],
                zm = loop_data.RealArray_2D_7xN[2],
                zp = loop_data.RealArray_2D_7xN[3],
                zq = loop_data.RealArray_2D_7xN[4],
                zr = loop_data.RealArray_2D_7xN[5],
                zu = loop_data.RealArray_2D_7xN[6],
                zv = loop_data.RealArray_2D_7xN[7],
                zz = loop_data.RealArray_2D_7xN[8];

            ref zrout = loop_data.RealArray_2D_7xN[9],
                zzout = loop_data.RealArray_2D_7xN[10];

            const t = 0.0037;
            const s = 0.0041;

            const kn = 6, jn = len;
            const kjDom = {1..kn-1, 1..jn-1};

            ltimer.start();
            for 0..#num_samples {
              for (k,j) in kjDom {
                za[k,j] = (zp[k+1,j-1] + zq[k+1,j-1] - zp[k,j-1] - zq[k,j-1]) *
                          (zr[k,j] + zr[k,j-1]) / (zm[k,j-1] + zm[k+1,j-1]);
                zb[k,j] = (zp[k,j-1] + zq[k,j-1] - zp[k,j] - zq[k,j]) *
                          (zr[k,j] + zr[k-1,j]) / (zm[k,j] + zm[k,j-1]);
              }
              for (k,j) in kjDom {
                zu[k,j] += s * (za[k,j]   * (zz[k,j] - zz[k,j+1]) -
                                za[k,j-1] * (zz[k,j] - zz[k,j-1]) -
                                zb[k,j]   * (zz[k,j] - zz[k-1,j]) +
                                zb[k+1,j] * (zz[k,j] - zz[k+1,j]));
                zv[k,j] += s * (za[k,j]   * (zr[k,j] - zr[k,j+1]) -
                                za[k,j-1] * (zr[k,j] - zr[k,j-1]) -
                                zb[k,j]   * (zr[k,j] - zr[k-1,j]) +
                                zb[k+1,j] * (zr[k,j] - zr[k+1,j]));
              }
              for (k,j) in kjDom {
                zrout[k,j] = zr[k,j] + t*zu[k,j];
                zzout[k,j] = zz[k,j] + t*zv[k,j];
              }
            }

            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.GEN_LIN_RECUR {
            loopInit(iloop, stat);
            ref b5 = loop_data.RealArray_1D[0],
                sa = loop_data.RealArray_1D[1],
                sb = loop_data.RealArray_1D[2];

            var stb5 = loop_data.RealArray_scalars[0];
            var kb5i = 0;
            ltimer.start();
            for 0..#num_samples {
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
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1],
                z = loop_data.RealArray_1D[2],
                u = loop_data.RealArray_1D[3],
                v = loop_data.RealArray_1D[4],
                w = loop_data.RealArray_1D[5],
                g = loop_data.RealArray_1D[6],
                xx = loop_data.RealArray_1D[7],
                vx = loop_data.RealArray_1D[9];
            const s = loop_data.RealArray_scalars[0],
                  t = loop_data.RealArray_scalars[1],
                  dk = loop_data.RealArray_scalars[2];
            ltimer.start();
            for 0..#num_samples {
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
            loopInit(iloop, stat);
            ref px = loop_data.RealArray_2D_Nx25[0],
                cx = loop_data.RealArray_2D_Nx25[1],
                vy = loop_data.RealArray_2D_64x64[0];
            ltimer.start();
            for 0..#num_samples {
              for k in 0..#25 {
                for i in 0..#25 {
                  for j in 0..#len {
                    px[j,i] += vy[k,i]*cx[j,k];
                  }
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.PLANCKIAN {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0],
                y = loop_data.RealArray_1D[1],
                u = loop_data.RealArray_1D[2],
                v = loop_data.RealArray_1D[3],
                w = loop_data.RealArray_1D[4];
            var expmax = 20.0;
            u[len-1] = 0.99 * expmax*v[len-1];
            ltimer.start();
            for 0..#num_samples {
              for k in 0..#len {
                y[k] = u[k] / v[k];
                w[k] = x[k] / (exp(y[k]) - 1.0);
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.IMP_HYDRO_2D {
            loopInit(iloop, stat);
            ref za = loop_data.RealArray_2D_7xN[0],
                zb = loop_data.RealArray_2D_7xN[1],
                zr = loop_data.RealArray_2D_7xN[2],
                zu = loop_data.RealArray_2D_7xN[3],
                zv = loop_data.RealArray_2D_7xN[4],
                zz = loop_data.RealArray_2D_7xN[5];

            ltimer.start();
            for 0..#num_samples {
              for j in 1..6-1 {
                for k in 1..len-1 {
                  var qa = za[j+1,k]*zr[j,k] + za[j-1,k]*zb[j,k] +
                       za[j,k+1]*zu[j,k] + za[j,k-1]*zv[j,k] + zz[j,k];
                  za[j,k] += 0.175*( qa - za[j,k] );
                }
              }
            }
            ltimer.stop();
            loopFinalize(iloop, stat, ilength);
          }
          when LoopKernelID.FIND_FIRST_MIN {
            loopInit(iloop, stat);
            ref x = loop_data.RealArray_1D[0];
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
            loop_data.RealArray_scalars[0] = 0.123*val*m;
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
