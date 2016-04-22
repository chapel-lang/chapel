use List;

var s1 : list(int) = makeList( 3, 1, 4, 1, 5, 9 );

writeln(s1);

for e in s1 {
  writeln(e);
}

s1.destroy();




var s2 : list(string) = makeList( "three", "one", "four" );

writeln(s2);

for e in s2 {
  writeln(e);
}

s2.destroy();
