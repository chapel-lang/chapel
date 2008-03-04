// Test tuple indefinite domain lookup

type key_t = (string, int, real);
var tupled: domain( key_t);
// var tupled: domain( (string, int, real));
tupled.add( ("pete", 1, 2.0));
tupled.add( ("beet", 9, 0.0));
tupled.add( ("cleat", 9, 0.0));
tupled.add( ("defeat", 9, 0.0));
tupled.add( ("feet", 3, 4.5));
tupled.add( ("seat", 6, 7.8));

def tcheck( s:string, i:int, f:real) {
  writeln( "member(", (s, i, f), ") = ", tupled.member( (s, i, f)));
}

tcheck( "meet", 5, 6.7);
tcheck( "pete", 1, 2.0);
tcheck( "pete", 1, 0.0);
tcheck( "pete", 2, 2.0);
tcheck( "feet", 3, 4.5);
tcheck( "seat", 6, 7.8);
