use parameters;
use domains;

/* Location of horizontal grid */
  var x : [D] real(rp);
  var y : [D] real(rp);

/* Horizontal grid spacing */
  const dx : real(rp) = Lx / nx;
  const dy : real(rp) = Ly / ny;

/* Zonal mean velocity profile and associated meridional PV gradient */
  var uBar : real(rp);
  var qyBar : real(rp);

/* Wavenumbers */
  var kx : [D_hat] real(rp);
  var ky : [D_hat] real(rp);
  var k2 : [D_hat] real(rp);
  var ik2 : [D_hat] real(rp);

/* Potential vorticity */
  var q : [D] real(rp);

/* Spectral potential vorticity */
  var q_hat : [D_hat] complex(cp);

/* Spectral streamfunction */
  var psi_hat : [D_hat] complex(cp);

/* Spectral velocities */
  var u_hat : [D_hat] complex(cp);
  var v_hat : [D_hat] complex(cp);

/* Physical space arrays */
  var q_phys : [D] real(rp);
  var u_phys : [D] real(rp);
  var v_phys : [D] real(rp);

/* For the Jacobian */
  var uq_hat : [D_hat] complex(cp);
  var vq_hat : [D_hat] complex(cp);
  var uq_phys : [D] real(rp);
  var vq_phys : [D] real(rp);

/* For the drag term */
  var drag_tmp : [D_hat] complex(cp);
  var drag_hat : [D_hat] complex(cp);
  var Uu_drag : [D] real(rp);
  var Uv_drag : [D] real(rp);

/* Arrays and variables for the ARK43 timestepping */
  var N1, N2, N3, N4, N5, N6 : [D_hat] complex(cp);
  var L1, L2, L3, L4, L5, L6 : [D_hat] complex(cp);
  var q_tmp : [D_hat] complex(cp);
  var Mq : [D_hat] complex(cp);
  var k8 : [D_hat] real(rp);
  var kn2 : [D_hat] real(rp);

  var err : [D] real(rp);
  var err_hat : [D_hat] complex(cp);

  var ae, ai : [ark2d] real(rp);
  var b, be : [ark1d] real(rp);

  // These will be only on Locale 0
  var err0, err1 : real(rp);
  var reject : bool = false;

/* For Leith */
  var kL : [D_hat] real(rp);
  var AL : real(rp);
  var qL_tmp : [D_hat] real(rp);
  var k_gt_forcing : [D_hat] real(rp);

/* For Smag */
  var kS : [D_hat] real(rp);
  var AS : real(rp);
  var qS_tmp : [D_hat] real(rp);
  var k_lt_forcing : [D_hat] real(rp);

/* For quadratic drag */
  var AD : real(rp);
  var qD_tmp : [D_hat] real(rp);

/* Temporary arrays for 1D FFTs */
  var tmp_f1 : [D_hat] complex(cp);
  var tmp_f1T : [D_hatT] complex(cp);
  var tmp_f2T : [D_hatT] complex(cp);

  var tmp_b1 : [D_hatT] complex(cp);
  var tmp_b1T : [D_hatT] complex(cp);
  var tmp_b2T : [D_hat] complex(cp);

/* Arrays for stochastic forcing */
  var gauss : [D] real(rp);
  var specr : [D_hat] real(rp);
  var speci : [D_hat] real(rp);
  var stoch_spec : [D_hat] complex(cp);
  var pert_spec : [D_hat] complex(cp);
  var pert_phys : [D] real(rp);
  var q_pert : [D_hat] complex(cp);

