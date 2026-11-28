use GpuDiagnostics;

proc doKernel(numtrials, numnodes, numgpus) {
  for 1..numtrials do
    coforall locIdx in 0..<min(numnodes, numLocales) do on Locales[locIdx] {
      coforall gpuId in 0..<min(numgpus, here.gpus.size) do
        on here.gpus[gpuId] {
          var A = [1, 2, 3, 4, 5];
          A += 1;
        }
    }
}

manage new gpuDiagnosticsManager(autoPrint=true) {
  doKernel(1, 2, 1);
}
writeln("finished\n");


var m = new gpuDiagnosticsManager();
writeln(m.get());
manage m {
  doKernel(1, 2, 1);
}
writeln("\nafter context");
writeln(m.get());

writeln("\nverbose");

manage new verboseGpuManager() {
  doKernel(1, 2, 1);
}
