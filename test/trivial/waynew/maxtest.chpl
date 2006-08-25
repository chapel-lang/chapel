// Test the max(type) primitive.

var i8:int(8);
var i16:int(16);
var i32:int(32);
var i64:int;
writeln( max( i8.type));
writeln( max( i16.type));
writeln( max( i32.type));
writeln( max( i64.type));
writeln( max( int.type));

var ui8:uint(8);
var ui16:uint(16);
var ui32:uint(32);
var ui64:uint;
writeln( max( ui8.type));
writeln( max( ui16.type));
writeln( max( ui32.type));
writeln( max( ui64.type));
writeln( max( uint.type));

var f32:float(32);
var f64:float;
writeln( max( f32.type));
writeln( max( f64.type));
writeln( max( float.type));

