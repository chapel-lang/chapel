use FFTW;

config var n = 256;

var A: [1..n, 1..n+1] complex;
var B: [1..n, 1..n] complex;

plan_dft(A, B, FFTW_FORWARD, FFTW_MEASURE);
