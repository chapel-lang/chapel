//Direct copy to GPUs

use CommDiagnostics;
use GPUDiagnostics;

config const n = 128;
config const alpha = 5;

var A, B, C: [0..<n] int;
B = 1;
C = 2;

startGPUDiagnostics();
coforall (l,lid) in zip(Locales, LocaleSpace) do on l {
  const perLocSize = n/numLocales;
  const locStart = lid*perLocSize;
  const locChunk = locStart..#perLocSize;

  const numGPUs = here.gpus.size; // initially I used `here.gpus.size` inside
                                  // the `on` below. But `here` on a gpu
                                  // sublocale is the sublocale. Should it be?
                                  // Probably. But maybe we need a way to query
                                  // number of siblings from a sublocale
  coforall (g,gid) in zip(here.gpus, here.gpus.domain) do on g {
    const perGPUSize = perLocSize/numGPUs;
    const gpuStart = locStart+gid*perGPUSize;
    const gpuChunk = gpuStart..#perGPUSize;

    var Ag: [gpuChunk] int;
    var Bg = B[gpuChunk], Cg = C[gpuChunk];

    Ag = Bg + alpha * Cg;

    A[gpuChunk] = Ag;
  }
}
stopGPUDiagnostics();

// validation
param nLaunch=1;
for l in Locales do
  assert(getGPUDiagnostics()[l.id].kernel_launch == nLaunch*l.gpus.size);
