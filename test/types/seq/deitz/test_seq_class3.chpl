class C {
  var x: int;
}

var s: seq of C;

writeln(s);

s #= C(2);

writeln(s);
