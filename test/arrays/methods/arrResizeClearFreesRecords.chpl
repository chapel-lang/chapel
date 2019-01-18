record R {
  proc init() {
    writeln("in init()");
  }

  proc deinit() {
    writeln("in deinit()");
  }
}

var D = {1..10};
var A: [D] R;
writeln(A);
D = {1..5};
writeln(A);
D.clear();
writeln(A);
