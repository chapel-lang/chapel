//Data created on Locales[0], chunks copied to locales and then to GPUs
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

  var Al: [locChunk] int;
  var Bl = B[locChunk], Cl = C[locChunk];

  const numGPUs = here.gpus.size;
  coforall (g,gid) in zip(here.gpus, here.gpus.domain) do on g {
    const perGPUSize = perLocSize/numGPUs;
    const gpuStart = locStart+gid*perGPUSize;
    const gpuChunk = gpuStart..#perGPUSize;

    var Ag: [gpuChunk] int;
    var Bg = Bl[gpuChunk], Cg = Cl[gpuChunk];

    Ag = Bg + alpha * Cg;
    Al[gpuChunk] = Ag;
  }

  A[locChunk] = Al;
}
stopGPUDiagnostics();

// validation
param nLaunch=1;
for l in Locales {
  assert(getGPUDiagnostics()[l.id].kernel_launch == nLaunch*l.gpus.size);
}
