// NAS FT - Ported to Chapel from ZPL

config const
  verbose         = true,
  timers_enabled  = false,
  timers_disabled = false;

const
  arand = 1220703125.0,
  seed  = 314159265.0,
  r23   = 0.5**23,
  t23   = 2.0**23,
  r46   = 0.5**46,
  t46   = 2.0**46;

def nextrandlc(x : float, a : float) {
  var t1 = r23 * a;
  var a1 = floor(t1);
  var a2 = a - t23 * a1;
  t1 = r23 * x;
  var x1 = floor(t1);
  var x2 = x - t23 * x1;
  t1 = a1 * x2 + a2 * x1;
  var t2 = floor(r23 * t1);
  var z  = t1 - t23 * t2;
  var t3 = t23 * z + a2 * x2;
  var t4 = floor(r46 * t3);
  var x3 = t3 - t46 * t4;
  return (x3, r46 * x3);
}

def initrandlc(seed, a : float, in n : int) : float {
  var i : int, t : float, g : float;
  var x : float = seed;
  t = a;
  while n != 0 do {
    i = n / 2;
    if 2 * i != n then
      (x, g) = nextrandlc(x, t);
    (t, g) = nextrandlc(t, t);
    n = i;
  }
  return x;
}

var randlc_last_n : int = -2,
    randlc_last_x : float;
def randlc(n : int) : float {
  var result : float;
  if n != randlc_last_n + 1 then
    randlc_last_x = initrandlc(seed, arand, n);
  randlc_last_n = n;
  (randlc_last_x, result) = nextrandlc(randlc_last_x, arand);
  return result;
}

enum classes {S = 1, W, A, B, C, D};

/*
var class_defaults : [S..D] (string, int, int, int, int) =
  (/ ('S',   64,   64,   64,  6),
     ('W',   32,  128,  128,  6),
     ('A',  128,  256,  512,  6),
     ('B',  256,  256,  512, 20),
     ('C',  512,  512,  512, 20),
     ('D', 1024, 1024, 2048, 25) /);
*/

config const problem_class = 1;

config var
  nx : int = 64,
  ny : int = 64,
  nz : int = 64,
  niter : int = 6;

const
  alpha = 0.000001,
  pi = 3.141592653589793238,
  epsilon = 0.000000000001;

var
  DXYZ : domain(3) = [0..nx-1, 0..ny-1, 0..nz-1],
  U0 : [DXYZ] complex,
  U1 : [DXYZ] complex,
  U2 : [DXYZ] complex,
  Twiddle : [DXYZ] float;

def compute_initial_conditions(X1) {
  forall i,j,k in DXYZ {
    X1(i,j,k).real = randlc(((i*ny+j)*nz+k)*2);
    X1(i,j,k).imag = randlc(((i*ny+j)*nz+k)*2+1);
  }
}

def compute_index_map(Twiddle) {
  const ap = -4.0 * alpha * pi * pi;
  forall i,j,k in DXYZ do
    Twiddle(i,j,k) = exp(ap*(((i+nx/2) % nx - nx/2)**2 +
                             ((j+ny/2) % ny - ny/2)**2 +
                             ((k+nz/2) % nz - nz/2)**2));
}

var fftblock : int = 16, fftblockpad : int = 18;

var u : [0..nz-1] complex;

def fft_init() {
  var t : float, ti : float;
  var m = bpop(nz-1);
  var ku = 2;
  var ln = 1;
  u(0) = m+0i;
  for j in 1..m {
    for i in 0..ln-1 {
      u(i+ku-1).real = cos(i*pi/ln);
      u(i+ku-1).imag = sin(i*pi/ln);
    }
    ku += ln;
    ln *= 2;
  }
}

def fftz2(dir, l, m, n, ny, ny1 : int, u, x, y) {
  var lk = 2**(l-1), li = 2**(m-1), lj = 2*lk;
  for i in 0..li-1 {
    var i11 = i * lk, i12 = i11 + n/2, i21 = i * lj, i22 = i21 + lk;
    var u1 = if dir == 1 then u(li+i) else conjg(u(li+i));
    for k in 0..lk-1 {
      for j in 0..ny-1 {
        var x11 = x(i11+k, j);
        var x21 = x(i12+k, j);
        y(i21+k, j) = x11+x21;
        y(i22+k, j) = u1*(x11-x21);
      }
    }
  }
}

def cfftz(dir, m, n, ny, ny1 : int, x, y) {
  for l in 1..m by 2 {
    fftz2(dir, l, m, n, ny, ny1, u, x, y);
    if l != m then
      fftz2(dir, l+1, m, n, ny, ny1, u, y, x);
    else
      forall ij in [0..n-1, 0..ny-1] do
        x(ij) = y(ij);
  }
}

def cffts1(dir, n, X1, X2, ny, ny1, x, y) {
  for j in DXYZ(2) {
    for kk in DXYZ(3) by ny {
      [i1,_,i3 in [0..n-1,j..j,kk..kk+ny-1]] x(i1,i3-kk) = X1(i1,j,i3);
      cfftz(dir, bpop(n-1), n, ny, ny1, x, y);
      [i1,_,i3 in [0..n-1,j..j,kk..kk+ny-1]] X2(i1,j,i3) = x(i1,i3-kk);
    }
  }
}

def cffts2(dir, n, X1, X2, ny, ny1, x, y) {
  for i in DXYZ(1) {
    for kk in DXYZ(3) by ny {
      [_,i2,i3 in [i..i,0..n-1,kk..kk+ny-1]] x(i2,i3-kk) = X1(i,i2,i3);
      cfftz(dir, bpop(n-1), n, ny, ny1, x, y);
      [_,i2,i3 in [i..i,0..n-1,kk..kk+ny-1]] X2(i,i2,i3) = x(i2,i3-kk);
    }
  }
}

def cffts3(dir, n, X1, X2, ny, ny1, x, y) {
  for i in DXYZ(1) {
    for jj in DXYZ(2) by ny {
      [_,i2,i3 in [i..i,jj..jj+ny-1,0..n-1]] x(i3,i2-jj) = X1(i,i2,i3);
      cfftz(dir, bpop(n-1), n, ny, ny1, x, y);
      [_,i2,i3 in [i..i,jj..jj+ny-1,0..n-1]] X2(i,i2,i3) = x(i3,i2-jj);
    }
  }
}

def fft(dir : int, X1, X2) {
  var x : [0..nz-1, 0..fftblockpad-1] complex;
  var y : [0..nz-1, 0..fftblockpad-1] complex;
  if dir == 1 {
    cffts3(dir, nz, X1, X1, fftblock, fftblockpad, x, y);
    cffts2(dir, ny, X1, X1, fftblock, fftblockpad, x, y);
    cffts1(dir, nx, X1, X2, fftblock, fftblockpad, x, y);
  } else {
    cffts1(dir, nx, X1, X1, fftblock, fftblockpad, x, y);
    cffts2(dir, ny, X1, X1, fftblock, fftblockpad, x, y);
    cffts3(dir, nz, X1, X2, fftblock, fftblockpad, x, y);
  }
}

def evolve(X1, X2, Twiddle) {
  forall ijk in DXYZ {
    X1(ijk) *= (Twiddle(ijk) + 0i);
    X2(ijk) = X1(ijk);
  }
}

var sums : [1..niter] complex; -- verification checksums

const vdata_s : [1..6] complex =
  (/554.6087004964+484.5363331978i,
    554.6385409189+486.5304269511i,
    554.6148406171+488.3910722336i,
    554.5423607415+490.1273169046i,
    554.4255039624+491.7475857993i,
    554.2683411902+493.2597244941i/);

var vdata_w : [1..6] complex;
vdata_w(1) = 567.3612178944+529.3246849175i;
vdata_w(2) = 563.1436885271+528.2149986629i;
vdata_w(3) = 559.4024089970+527.0996558037i;
vdata_w(4) = 556.0698047020+526.0027904925i;
vdata_w(5) = 553.0898991250+524.9400845633i;
vdata_w(6) = 550.4159734538+523.9212247086i;

var vdata_a : [1..6] complex;
vdata_a(1) = 504.6735008193+511.4047905510i;
vdata_a(2) = 505.9412319734+509.8809666433i;
vdata_a(3) = 506.9376896287+509.8144042213i;
vdata_a(4) = 507.7892868474+510.1336130759i;
vdata_a(5) = 508.5233095391+510.4914655194i;
vdata_a(6) = 509.1487099959+510.7917842803i;

var vdata_b : [1..20] complex;
vdata_b(1) = 517.7643571579+507.7803458597i;
vdata_b(2) = 515.4521291263+508.8249431599i;
vdata_b(3) = 514.6409228649+509.6208912659i;
vdata_b(4) = 514.2378756213+510.1023387619i;
vdata_b(5) = 513.9626667737+510.3976610617i;
vdata_b(6) = 513.7423460082+510.5948019802i;
vdata_b(7) = 513.5547056878+510.7404165783i;
vdata_b(8) = 513.3910925466+510.8576573661i;
vdata_b(9) = 513.2470705390+510.9577278523i;
vdata_b(10) = 513.1197729984+511.0460304483i;
vdata_b(11) = 513.0070319283+511.1252433800i;
vdata_b(12) = 512.9070537032+511.1968077718i;
vdata_b(13) = 512.8182883502+511.2616233064i;
vdata_b(14) = 512.7393733383+511.3203605551i;
vdata_b(15) = 512.6691062020+511.3735928093i;
vdata_b(16) = 512.6064276004+511.4218460548i;
vdata_b(17) = 512.5504076570+511.4656139760i;
vdata_b(18) = 512.5002331720+511.5053595966i;
vdata_b(19) = 512.4551951846+511.5415130407i;
vdata_b(20) = 512.4146770029+511.5744692211i;

var vdata_c : [1..20] complex;
vdata_c(1) = 519.5078707457+514.9019699238i;
vdata_c(2) = 515.5422171134+512.7578201997i;
vdata_c(3) = 514.4678022222+512.2251847514i;
vdata_c(4) = 514.0150594328+512.1090289018i;
vdata_c(5) = 513.7550426810+512.1143685824i;
vdata_c(6) = 513.5811056728+512.1496764568i;
vdata_c(7) = 513.4569343165+512.1870921893i;
vdata_c(8) = 513.3651975661+512.2193250322i;
vdata_c(9) = 513.2955192805+512.2454735794i;
vdata_c(10) = 513.2410471738+512.2663649603i;
vdata_c(11) = 513.1971141679+512.2830879827i;
vdata_c(12) = 513.1605205716+512.2965869718i;
vdata_c(13) = 513.1290734194+512.3075927445i;
vdata_c(14) = 513.1012720314+512.3166486553i;
vdata_c(15) = 513.0760908195+512.3241541685i;
vdata_c(16) = 513.0528295923+512.3304037599i;
vdata_c(17) = 513.0310107773+512.3356167976i;
vdata_c(18) = 513.0103090133+512.3399592211i;
vdata_c(19) = 512.9905029333+512.3435588985i;
vdata_c(20) = 512.9714421109+512.3465164008i;

var vdata_d : [1..25] complex;
vdata_d(1) = 512.2230065252+511.8534037109i;
vdata_d(2) = 512.0463975765+511.7061181082i;
vdata_d(3) = 511.9865766760+511.7096364601i;
vdata_d(4) = 511.9518799488+511.7373863950i;
vdata_d(5) = 511.9269088223+511.7680347632i;
vdata_d(6) = 511.9082416858+511.7967875532i;
vdata_d(7) = 511.8943814638+511.8225281841i;
vdata_d(8) = 511.8842385057+511.8451629348i;
vdata_d(9) = 511.8769435632+511.8649119387i;
vdata_d(10) = 511.8718203448+511.8820803844i;
vdata_d(11) = 511.8683569061+511.8969781011i;
vdata_d(12) = 511.8661708593+511.9098918835i;
vdata_d(13) = 511.8649768950+511.9210777066i;
vdata_d(14) = 511.8645605626+511.9307604484i;
vdata_d(15) = 511.8647586618+511.9391362671i;
vdata_d(16) = 511.8654451572+511.9463757241i;
vdata_d(17) = 511.8665212451+511.9526269238i;
vdata_d(18) = 511.8679083821+511.9580184108i;
vdata_d(19) = 511.8695433664+511.9626617538i;
vdata_d(20) = 511.8713748264+511.9666538138i;
vdata_d(21) = 511.8733606701+511.9700787219i;
vdata_d(22) = 511.8754661974+511.9730095953i;
vdata_d(23) = 511.8776626738+511.9755100241i;
vdata_d(24) = 511.8799262314+511.9776353561i;
vdata_d(25) = 511.8822370068+511.9794338060i;

def verify() {
  var rerr : float, ierr : float;
  for iter in 1..niter {
    select problem_class {
      when S {
        rerr = (sums(iter).real - vdata_s(iter).real) / vdata_s(iter).real;
        ierr = (sums(iter).imag - vdata_s(iter).imag) / vdata_s(iter).imag;
      }
      when W {
        rerr = (sums(iter).real - vdata_w(iter).real) / vdata_w(iter).real;
        ierr = (sums(iter).imag - vdata_w(iter).imag) / vdata_w(iter).imag;
      }
      when A {
        rerr = (sums(iter).real - vdata_a(iter).real) / vdata_a(iter).real;
        ierr = (sums(iter).imag - vdata_a(iter).imag) / vdata_a(iter).imag;
      }
      when B {
        rerr = (sums(iter).real - vdata_b(iter).real) / vdata_b(iter).real;
        ierr = (sums(iter).imag - vdata_b(iter).imag) / vdata_b(iter).imag;
      }
      when C {
        rerr = (sums(iter).real - vdata_c(iter).real) / vdata_c(iter).real;
        ierr = (sums(iter).imag - vdata_c(iter).imag) / vdata_c(iter).imag;
      }
      when D {
        rerr = (sums(iter).real - vdata_d(iter).real) / vdata_d(iter).real;
        ierr = (sums(iter).imag - vdata_d(iter).imag) / vdata_d(iter).imag;
      }
    }
    if !(abs(rerr) <= epsilon && abs(ierr) <= epsilon) then
      return false;
  }
  return true;
}

def checksum(i, X1) {
  var chk : complex;
  [j in 1..1024] chk += X1((5*j) % nx, (3*j) % ny, j % nz);
  chk = chk / ((nx * ny * nz) + 0i);
  sums(i) = chk;
  if verbose then
    writeln("T = ", i, "    Checksum = ", sums(i).real, " ", sums(i).imag);
}

--
-- Run problem once to ensure all data is touched
--

compute_index_map(Twiddle);
compute_initial_conditions(U1);
fft_init();
fft(1, U1, U0);

--
-- Restart benchmark
--

compute_index_map(Twiddle);
compute_initial_conditions(U1);
fft_init();
fft(1, U1, U0);

for iter in 1..niter {
  evolve(U0, U1, Twiddle);
  fft(-1, U1, U2);
  checksum(iter, U2);
}

if verify() {
  writeln("Result verification successful.");
} else {
  writeln("Result verification failed.");
}
