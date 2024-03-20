on here.gpus[0] {
  var x = 0;
  forall i in 0..10 with (+ reduce x) {
    x += i;
  }
  writeln(x);
}
