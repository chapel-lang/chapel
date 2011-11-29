iter foo_for (param rank : int) : int {
  for j in 1..rank {
    yield j;
  }
}

for i in foo_for( 10) {
  writeln((i));
}

