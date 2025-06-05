use Math;

/* Precision */
config param rp = 64;   // real precision
config param cp = 128;  // complex precision

/* Restart? */
config const restart : bool = false;
config const restart_file : string;

/* Background settings */
config const background_file = "background_2D.nc1";

/* Timestepping */
config var Nt_start : int = 0;
config var Nt : int = 10;
config var t : real(rp) = 0;
config var dt : real(rp) = 6e2;

/* Domain, meters */
config const Lx : real(rp) = 1.008e7;
config const Ly : real(rp) = 1.008e7;

/* Grid: Horizontal part must be divisible by 3. */
config const nx : int  = 24;
config const ny : int  = 24;

var nx3p : int = (nx/3)+1;
var ny3p : int = (ny/3)+1;
var nx2p : int = (nx/2)+1;
var ny2p : int = (ny/2)+1;
var ny3p2 : int = (2*(ny/3)+1);

/* Coriolis coefficients */
config const beta : real(rp) = 0.0;

/* Viscosities */
config const A2 : real(rp) = 88;
config const Leith_order : real(rp) = 8;
config const Leith_coeff : real(rp) = 0;
config const Smag_order : real(rp) = -8;
config const Smag_coeff : real(rp) = 0.0;

/* Forcing wavenumber */
config const forcing_k : real(rp);

/* F0 is the enstrophy injection rate (s-2) */
config const F0 : real(rp) = 1.75e-13;

/* Quadratic drag parameter. C_d = c_d/h_BL where h_BL is the bottom
   boundary layer thickness. This is the drag felt by the barotropic mode,
   not by the bottom layer. C_d*L_d is a crude measure of the strength of the drag.
   > 1 is strong drag, < 1 is weak drag. */
config const C_d : real(rp) = 1.25e-8;

/* Time step error tolerance; max-norm vorticity */
config const TOL : real(rp) = 1e-8;

/* CFL for the fastest Rossby wave = 1 / (10 * frequency) */
config const dt_max : real(rp) = 0.2 * pi / (beta * Lx);
