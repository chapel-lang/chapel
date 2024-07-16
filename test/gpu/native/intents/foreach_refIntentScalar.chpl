on here.gpus[0] {
  var x = 1;
  writeln("Before loop x = ", x);
  @assertOnGpu
  foreach 0..0 with (ref x) do x = 2;
  writeln("After loop x = ", x);
}
