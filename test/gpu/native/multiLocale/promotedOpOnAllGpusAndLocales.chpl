// Everything is initiated on the GPU(s)
use GpuDiagnostics;

config const n = 10;
config const alpha = 5;

startGpuDiagnostics();
coforall l in Locales do on l {
  coforall g in here.gpus do on g {
    var A, B, C: [1..n] int;

    B = 1;
    C = 2;
    A = B + alpha  * C;
  }
}
stopGpuDiagnostics();

// validation
param nLaunch=3;
for l in Locales do
  assert(getGpuDiagnostics()[l.id].kernel_launch == nLaunch*l.gpus.size);
