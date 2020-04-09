// Test the max(type) primitive.

var i8:int(8);
var i16:int(16);
var i32:int(32);
var i64:int(64);
writeln( max( i8.type));
writeln( max( i16.type));
writeln( max( i32.type));
writeln( max( i64.type));
writeln( max( int(64)));

var ui8:uint(8);
var ui16:uint(16);
var ui32:uint(32);
var ui64:uint(64);
writeln( max( ui8.type));
writeln( max( ui16.type));
writeln( max( ui32.type));
writeln( max( ui64.type));
writeln( max( uint(64)));

var f32:real(32);
var f64:real;
writeln( max( f32.type));
writeln( max( f64.type));
writeln( max( real));

var c1:complex(64);
var c2:complex(128);
writeln( max( c1.type));
writeln( max( c2.type));

