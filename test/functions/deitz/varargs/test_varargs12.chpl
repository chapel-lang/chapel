proc bar(x...) {
  writeln(x.size);
  writeln(x);
}

bar(1);
bar(1,2);
bar("three",4,5.0);
