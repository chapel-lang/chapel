// Test the identity values for product, logical and, logical or,
// logical xor, bit-wise and, bit-wise or, and bitwise xor.

proc identities( type t) {
  writeln( "product = ", _prod_id( t));
  writeln( "logical and = ", _land_id( t));
  writeln( "logical or = ", _lor_id( t));
  writeln( "logical xor = ", _lxor_id( t));
  writeln( "bit-wise and = ", _band_id( t));
  writeln( "bit-wise or = ", _bor_id( t));
  writeln( "bit-wise xor = ", _bxor_id( t));
}


var i8:int(8);
writeln( "        int(i8)");
identities( i8.type);
var i16:int(16);
writeln( "        int(16)");
identities( i16.type);
var i32:int(32);
writeln( "        int(32)");
identities( i32.type);
var i64:int;
writeln( "        int(64)");
identities( i64.type);


var ui8:uint(8);
writeln( "        uint(i8)");
identities( ui8.type);
var ui16:uint(16);
writeln( "        uint(16)");
identities( ui16.type);
var ui32:uint(32);
writeln( "        uint(32)");
identities( ui32.type);
var ui64:int;
writeln( "        uint(64)");
identities( ui64.type);


var f32:real(32);
writeln( "        real(32)");
identities( f32.type);
var f64:real;
writeln( "        real(64)");
identities( f64.type);
