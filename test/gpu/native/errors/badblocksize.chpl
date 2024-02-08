use GPU;

on here.gpus[0] {
  var A: [1..10] int;

  setBlockSize(1024);
  foreach a in A do a+= 1;
}
