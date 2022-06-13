// Everything is initiated on the GPU(s)
use GPUDiagnostics;

config const n = 10;
config const alpha = 5;

startGPUDiagnostics();
coforall l in Locales do on l {
  coforall g in here.gpus do on g {
    var A, B, C: [1..n] int;

    B = 1;
    C = 2;
    A = B + alpha  * C;
  }
}
stopGPUDiagnostics();

// validation
param nLaunch=3;
for l in Locales do
  assert(getGPUDiagnostics()[l.id].kernel_launch == nLaunch*l.gpus.size);
