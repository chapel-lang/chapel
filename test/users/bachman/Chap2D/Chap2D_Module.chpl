use IO;
use FFTW;
use C_FFTW;
use CTypes;

use Math;
use LinearAlgebra;
use Random;

use parameters;
use domains;
use arrays;
use FFT_utils;
use ARK43;
use IO_Module;

use compare_fortran;
use Time;

////////////////////////////////////////////////////////////////
//  Initialize: Set up the initial PV field, modes, and FFTs  //
////////////////////////////////////////////////////////////////

proc Initialize() {

  if restart {
    read_initial_state(q);
  }
  else {
    create_initial_state(q);
  }

  read_background_state(background_file);


  writeln("---------------------------------------------------------------");
  writeln("dx = ", dx,"                dy = ", dy);

  /* Horizontal locations of grid points (physical space) */
    forall (i,j) in D {
        x[i,j] = -(Lx+dx)/2 + dx*(j+1);
    }

    forall (i,j) in D {
        y[i,j] = -(Ly+dy)/2 + dy*(i+1);
    }

    /* Initialize wavenumbers. We are going to do all spectral operations on the
       transposed domain, so kx will be constant in the first dimension and ky
       will be constant in the second dimension. */

      // These loops will only affect parts of kx, ky that
      // are in the local subdomain
      for i in 0..#nx2p {
        kx[..,i] = (2*pi/Lx) * i;
      }
      for j in 0..#ny2p {
        ky[j,..] = (2*pi/Ly) * j;
      }
      for j in ny2p..(ny-1) {
        ky[j,..] = (2*pi/Ly) * (j-ny);
      }

      k2 = kx**2+ky**2;
      ik2 = 1.0 / k2;
      ik2[0,0] = 0;

    /* Initialize mask for stochastic forcing */
      set_StochSpec(stoch_spec);

    /* Set up Fourier Transforms */
      set_up_forward_FFTs();
      set_up_backward_FFTs();

    /* Transform the initial condition */
      writeln(" Transforming the initial condition ");
      writeln("---------------------------------------------------------------");

      execute_forward_FFTs(q, q_hat);

      DeAlias(q_hat);

    /* Set the arrays for the ARK43 timestepping scheme on each Locale */
      writeln(" Setting ARK43 arrays ");
      writeln("---------------------------------------------------------------");
      set_ARK43_vars();


      writeln(" Initialization complete ");
      writeln("---------------------------------------------------------------");

}


////////////////////////////////////////////////////////////////
//          DeAlias: zeros out top 1/3 of coefficients        //
////////////////////////////////////////////////////////////////

proc DeAlias(ref field : [] complex(cp)) {

    forall (j,k) in D_hat_sp1 {
      field[j,k] = 0;
    }
    forall (j,k) in D_hat_sp2 {
      field[j,k] = 0;
    }
    field[0,0] = 0;
}

////////////////////////////////////////////////////////////////
//                          Jacobian                          //
//         Computes 2/3-rule dealiased jacobian, returns      //
//           Fourier coefficients thereof in jaco_spec        //
//       Also computes spectral coefficients of v for beta    //
//                  and of psi for viscosity                  //
//                   Assumes dealiased input                  //
////////////////////////////////////////////////////////////////

proc Jacobian(ref q_in : [] complex(cp), ref jaco_hat : [] complex(cp)) {

  /* Get psi_hat, u_hat, v_hat */
    GetPsi(q_in);

    forall (j,k) in D_hat {
      u_hat[j,k] = -1i*ky[j,k]*psi_hat[j,k];
      v_hat[j,k] = 1i*kx[j,k]*psi_hat[j,k];
    }

  /* Get u, v, q */
    execute_backward_FFTs(q_in, q_phys);
    normalize(q_phys);

    execute_backward_FFTs(u_hat, u_phys);
    normalize(u_phys);

    execute_backward_FFTs(v_hat, v_phys);
    normalize(v_phys);


    forall (j,k) in D {
      uq_phys[j,k] = u_phys[j,k]*q_phys[j,k];
      vq_phys[j,k] = v_phys[j,k]*q_phys[j,k];
    }

  /* Get uq_hat, vq_hat */
    execute_forward_FFTs(uq_phys, uq_hat);
    execute_forward_FFTs(vq_phys, vq_hat);

  /* Compute jacobian_spec */
  /* The RHS term is the negative of the Jacobian, so I will put a
     minus sign here to avoid a needless array copy in the calling function. */
    forall (j,k) in D_hat {
      jaco_hat[j,k] = -1i*(kx[j,k]*uq_hat[j,k]+ky[j,k]*vq_hat[j,k]);
    }

}

////////////////////////////////////////////////////////////////
//                  GetRHS: computes FFT of                   //
//     -J[psi,q]-uBar*qx-(beta+qyBar)*v-nu*del[q]             //
////////////////////////////////////////////////////////////////

proc GetRHS(ref q_in : [] complex(cp), ref RHS : [] complex(cp)) {

  /* Advection */
  Jacobian(q_in,RHS);

  /* Mean advection, beta and viscosity */
    forall (j,k) in D_hat {
      RHS[j,k] = RHS[j,k] - uBar*1i*kx[j,k]*q_in[j,k]
                 - (beta + qyBar)*v_hat[j,k];
    }

  /* Quadratic drag */
    forall (j,k) in D {
      Uu_drag[j,k] = sqrt(u_phys[j,k]**2+v_phys[j,k]**2)*u_phys[j,k];
      Uv_drag[j,k] = sqrt(u_phys[j,k]**2+v_phys[j,k]**2)*v_phys[j,k];
    }

    execute_forward_FFTs(Uu_drag, drag_tmp);
    forall (j,k) in D_hat {
      drag_hat[j,k] = 1i*ky[j,k]*drag_tmp[j,k];
    }

    execute_forward_FFTs(Uv_drag, drag_tmp);
    forall (j,k) in D_hat {
      drag_hat[j,k] = drag_hat[j,k] - 1i*kx[j,k]*drag_tmp[j,k];
    }

    forall (j,k) in D_hat {
      RHS[j,k] = RHS[j,k] + C_d*drag_hat[j,k];
    }

  /* Dealias */
    DeAlias(RHS);

}


////////////////////////////////////////////////////////////////
//              GetPsi: Gets psi_hat from q_hat               //
////////////////////////////////////////////////////////////////

proc GetPsi(ref in_arr : [] complex(cp)) {

  forall (j,k) in D_hat {
    psi_hat[j,k] = in_arr[j,k]/(-k2[j,k]);
  }
  psi_hat[0,0] = 0;

}


////////////////////////////////////////////////////////////////
//   create_initial_state: Fill initial q with random values  //
////////////////////////////////////////////////////////////////

proc create_initial_state(ref in_arr : [?dom] real(rp)) {

  var D = dom.localSubdomain();
  var tmp = in_arr.localSlice(D);

  for (j,k) in D {
    tmp[j,k] = 1e-5*sin(7.0*pi*(k-1) / nx) : real(rp);
    tmp[j,k] = tmp[j,k] + 2e-5*cos(14.0*pi*(j-1) / ny) : real(rp);
  }

  in_arr[D] = tmp;

}


////////////////////////////////////////////////////////////////
//    rand_normal: generates samples from a standard normal   //
////////////////////////////////////////////////////////////////

proc rand_normal(ref gauss : [?dom] real(rp)) {

  var tmp1  : [0..<ny, 0..<(nx/2)] real(rp);
  var tmp2  : [0..<ny, 0..<(nx/2)] real(rp);
  var r     : [0..<ny, 0..<(nx/2)] real(rp);
  var theta : [0..<ny, 0..<(nx/2)] real(rp);

  fillRandom(tmp1);
  fillRandom(tmp2);
  r = sqrt(-2.0 * log(1.0 - tmp1));
  theta = 2.0*pi*tmp2;

  gauss[..,0..<(nx/2)] = r*sin(theta);
  gauss[..,(nx/2)..] = r*cos(theta);

}

////////////////////////////////////////////////////////////////
//     StochPert: generates a stochastic perturbation to      //
//     the vorticity field                                    //
////////////////////////////////////////////////////////////////

proc StochPert(ref zeta_hat : [?dom] complex(cp)) {

  // Generate forcing Fourier coefficients
  rand_normal(gauss);
  specr = gauss[..,0..<(nx/2+1)];
  speci = 0;
  speci[..,1..<(nx/2)] = gauss[..,(nx/2+1)..];
  pert_spec = stoch_spec * (specr + 1i*speci);

  // Laplacian to get q perturbation
  zeta_hat = -k2 * pert_spec;
  zeta_hat[0,0] = 0;

}


////////////////////////////////////////////////////////////////
//     StochPert: generates a stochastic perturbation to      //
//     the vorticity field                                    //
////////////////////////////////////////////////////////////////

proc set_StochSpec(ref ss : [?dom] complex(cp)) {

  // Set wavenumber range for stochastic perturbation;
  // kx = (2*pi/Lx) * i, where i is an integer
  var kMin_domain = (2*pi / Lx)*1;
  var kMax_domain = (2*pi / Lx)*nx2p;

  writeln("kMin_domain (log10) is: ", log10(kMin_domain));
  writeln("kMax_domain (log10) is: ", log10(kMax_domain));

  var middle_k_domain = (log10(kMax_domain) + log10(kMin_domain)) / 2;
  var forcing_wavenumber = 10**(middle_k_domain);

  /* This is the near-integer wavenumber that I will force at */
  var k : real;
  if (forcing_k > 0) {
    k = forcing_k;
  } else {
    k = forcing_wavenumber / kMin_domain;
  }

  var kp = ceil(k);
  var km = floor(k);

  var forcing_kp = kp*(2*pi/Lx);
  var forcing_km = km*(2*pi/Lx);

  writeln("Forcing wavenumber (lambda / kMin): ", k);

  forall (j,k) in D_hat {
    if ( ((forcing_km**2)<=k2[j,k]) && (k2[j,k]<=(forcing_kp**2)) ) {
      ss[j,k] = 1.0 / sqrt(k2[j,k]);
    }
  }
  ss[0,0] = 0;

  // For Smag
  forall (j,k) in D_hat {
    if (k2[j,k]<=(forcing_km**2)) {
      k_lt_forcing[j,k] = 1.0;
    }
  }

  // For Leith
  forall (j,k) in D_hat {
    if ((forcing_kp**2)<=k2[j,k]) {
      k_gt_forcing[j,k] = 1.0;
    }
  }

}
