var n = 5;

for i in 1..n {
  for j in 1..i {
    writeln(i-j+1, ", ", j);
  }
  writeln();
}

for i in 2..n {
  for j in i..n {
    writeln(n-j+i, ", ", j);
  }
  writeln();
}
