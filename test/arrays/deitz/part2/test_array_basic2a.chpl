var d : domain(1) = {1..5};
var a : [d] real;

writeln(a);

[i in 1..5] {
  a(i) = i * 1.0;
}

writeln(a);

writeln(a(1));
