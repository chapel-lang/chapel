use GpuDiagnostics;
use IO only stderr;

stderr.writeln("global start/stop");

startVerboseGpu();
startVerboseGpu(); // triggers warning
stopVerboseGpu();
stopVerboseGpu(); // trigers warning

startGpuDiagnostics();
startGpuDiagnostics(); // triggers warning
stopGpuDiagnostics();
stopGpuDiagnostics(); // trigers warning
