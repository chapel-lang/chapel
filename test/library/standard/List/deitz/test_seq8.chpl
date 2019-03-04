use LinkedLists;

var s1 : linkedList(int) = makeList( 3, 1, 4, 1, 5, 9 );

writeln(s1);

for e in s1 {
  writeln(e);
}

s1.destroy();




var s2 : linkedList(string) = makeList( "three", "one", "four" );

writeln(s2);

for e in s2 {
  writeln(e);
}

s2.destroy();
