// Test tuple indefinite domain lookup

type my_key_t = (string, int, real);
var tupled: domain(my_key_t);
// var tupled: domain( (string, int, real));
tupled.add( ("pete", 1, 2.0));
tupled.add( ("beet", 9, 0.0));
tupled.add( ("cleat", 9, 0.0));
tupled.add( ("defeat", 9, 0.0));
tupled.add( ("feet", 3, 4.5));
tupled.add( ("seat", 6, 7.8));

proc tcheck( s:string, i:int, f:real) {
  writeln( "contains(", (s, i, f), ") = ", tupled.contains( (s, i, f)));
}

tcheck( "meet", 5, 6.7);
tcheck( "pete", 1, 2.0);
tcheck( "pete", 1, 0.0);
tcheck( "pete", 2, 2.0);
tcheck( "feet", 3, 4.5);
tcheck( "seat", 6, 7.8);
