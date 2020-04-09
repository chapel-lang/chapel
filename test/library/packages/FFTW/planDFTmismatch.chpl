use FFTW;

config var n = 50, m = 100;

config param version = 1;

var Ac: [1..n, 1..m] complex;
var Bc: [1..n+1, 1..m+1] complex;
var Cr: [1..n+2, 1..m+2] real;
var Dr = {1..n+3, 1..m+3};
var Ei: [1..n+4, 1..m+4] int;
var Fr32: [1..n+5, 1..m+5] real(32);

if version == 1 then
  plan_dft(Ac, Bc, FFTW_FORWARD, FFTW_MEASURE);

if version == 2 then
  plan_dft_r2c(Cr, Ac, FFTW_MEASURE);

if version == 3 then
  plan_dft_r2c(Dr, Ac, FFTW_MEASURE);

if version == 4 then
  plan_dft_r2c(Dr, Cr, FFTW_MEASURE);

if version == 5 then
  plan_dft_c2r(Ac, Cr, FFTW_MEASURE);

if version == 6 then
  plan_dft_c2r(Dr, Ac, FFTW_MEASURE);

if version == 7 then
  plan_dft_c2r(Dr, Cr, FFTW_MEASURE);

if version == 8 then
  plan_dft_r2c(Dr, Ei, FFTW_MEASURE);

if version == 9 then
  plan_dft_r2c(Dr, Fr32, FFTW_MEASURE);

if version == 10 then
  plan_dft_c2r(Dr, Ei, FFTW_MEASURE);

if version == 11 then
  plan_dft_c2r(Dr, Fr32, FFTW_MEASURE);
