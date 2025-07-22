use Print;

record R {
  var x : int;
  var y : real;
}

proc main() {
  var r1 : R;
  var r2 : R;

  // avoid 'new' for now, triggers big call graph
  r1.x = 1;
  r1.y = 2.0;
  r2.x = 3;
  r2.y = 4.0;
  
  r1 = r2;

  println(r1.x);
  println(r1.y);
}
