use GPU;

writeln(gpuClock());

on here.gpus[0] {
  writeln(gpuClock()); // still on host
}
