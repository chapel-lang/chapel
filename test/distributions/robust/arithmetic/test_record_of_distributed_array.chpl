use driver;

record R {
  var dom: domain(2) distributed Dist2D = [1..5, 1..5];
  var arr: [dom] int;
}

var r: R;

forall i in r.dom do
  r.arr(i) = next();

writeln(r);
