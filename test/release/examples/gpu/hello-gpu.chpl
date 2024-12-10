use GpuDiagnostics;
use ChplConfig;

config const N = 32;
config const verbose = false;

var correct = true;
const numGpusPerLocale = here.gpus.size;

startGpuDiagnostics();

coforall loc in Locales with (&& reduce correct) do on loc {
  coforall gpuId in 0..<numGpusPerLocale with (&& reduce correct) {
    var AOnCpu : [1..#N] int;

    on here.gpus[gpuId] {
      var AOnGpu = AOnCpu;
      @assertOnGpu
      foreach i in 1..#N do AOnGpu[i] = i;
      AOnCpu = AOnGpu;
    }

    if verbose then
      writeln("Hello from ", loc, " gpu: ", gpuId);

    correct &&= (+ reduce AOnCpu) == (N * (N + 1)) / 2;
  }
}

stopGpuDiagnostics();
const diags = getGpuDiagnostics();

forall i in 0..<Locales.size with (&& reduce correct) {
  if verbose then
    writeln("On locale ", i, " diags = ", diags[i]);

  if CHPL_GPU != "cpu" then
    correct &&=
      diags[i].kernel_launch  == numGpusPerLocale &&
      diags[i].host_to_device == numGpusPerLocale &&
      diags[i].device_to_host == numGpusPerLocale;
  else
    correct &&=
      diags[i].kernel_launch  == numGpusPerLocale &&
      diags[i].host_to_device == 0 &&
      diags[i].device_to_host == 0;
}

writeln("CORRECT = ", correct);
