var a : [1..5] real = 0.0;

writeln(a);

[i in 1..5] {
  a(i) = i;
}

writeln(a);

writeln(a(1));
