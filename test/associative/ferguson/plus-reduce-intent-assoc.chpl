// Can use + reduce intent on associative domain?
var D: domain(int);

forall x in 1..10 with (+ reduce D) {
  D += x;
}

writeln(D.sorted());
