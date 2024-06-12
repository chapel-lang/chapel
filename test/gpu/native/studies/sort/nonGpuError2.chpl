use GPU;
if (here.gpus.size < 2) {
  writeln("This program requires at least two GPUs.");
  halt();
}
var gpu1 = here.gpus[1];
var A: [1..100] uint;
on here.gpus[0] {
  on gpu1 {
    gpuSort(A);
  }
}
