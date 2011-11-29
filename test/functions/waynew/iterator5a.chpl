iter foo_while( param rank : int) : int {
  var j: int;
  j = 1;
  while (j <= rank) {
    yield j;
    j += 1;
  }
}

for i in foo_while( 10) {
  writeln((i));
}

