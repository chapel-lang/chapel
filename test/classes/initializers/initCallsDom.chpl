var a = [1, 2, 3];

record R {
  var d: domain(1);
  var a: [d] real;

  proc init(x) where !x: R {
    d = x.domain;
    this.initDone();
    for i in d do
      a[i] = i/10.0;
  }
}

var myR = new R(a);
writeln(myR);
