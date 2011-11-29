iter foo_for( rank: int) : int {
  for j in 1..rank {
    yield j;
  }
}


for i in foo_for( 10) {
  write( " ", i);
}
writeln();

writeln( foo_for( 10));
