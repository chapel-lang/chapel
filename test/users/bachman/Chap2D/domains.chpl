use parameters;

////////////////////////////////////////////////////////////
//   NOTE: All spectral arrays will be transposed with    //
//   respect to their physical space counterparts. This   //
//   is to enable the code to use serial 1D FFTs.         //
////////////////////////////////////////////////////////////

/* Horizontal domain (physical space) */
var D = {0..#ny,0..#nx};
var DT = {0..#nx, 0..#ny};

/* Horizontal domain (spectral space) */
var D_hat = {0..#ny, 0..#nx2p};
var D_hatT = {0..#nx2p,0..#ny};

/* Special domains for 1D FFTs */
var D_nx = {0..#nx};
var D_ny = {0..#ny};
var D_nx2p = {0..#nx2p};
var D_ny2p = {0..#ny2p};

/* Special domains for DeAliasing */
var D_hat_sp1 = {0..#ny, (nx3p-1)..(nx2p-1)};
var D_hat_sp2 = {(ny3p-1)..(ny3p2-1), 0..#nx2p};

/* Special domains for ARK43 */
var ark1d = {1..6};
var ark2d = {1..6, 1..6};

