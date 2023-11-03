use GPU;
use Random;

proc test(ref Arr) {
  fillRandom(Arr, min=0, max=100, seed=17);

  writeln(Arr);
  writeln(gpuMinLocReduce(Arr));
  writeln(gpuMaxLocReduce(Arr));
}

on here.gpus[0] {
  var A: [-20..#10] int;
  test(A);

  var B: [0..#10] int;
  test(B);

  var C: [10..#10] int;
  test(C);
}
