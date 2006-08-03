// Test the _min(type) primitive.

var i8:int(8);
var i16:int(16);
var i32:int(32);
var i64:int;
writeln( _min( i8.type));
writeln( _min( i16.type));
writeln( _min( i32.type));
writeln( _min( i64.type));
writeln( _min( int.type));

var ui8:uint(8);
var ui16:uint(16);
var ui32:uint(32);
var ui64:uint;
writeln( _min( ui8.type));
writeln( _min( ui16.type));
writeln( _min( ui32.type));
writeln( _min( ui64.type));
writeln( _min( uint.type));

var f32:float(32);
var f64:float;
writeln( _min( f32.type));
writeln( _min( f64.type));
writeln( _min( float.type));

/*
var c32:complex(32);
var c64:complex;
writeln( _min( c32.type));
writeln( _min( c64.type));
writeln( _min( complex.type));
*/
