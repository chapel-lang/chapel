// Test the min(type) primitive.

var i8:int(8);
var i16:int(16);
var i32:int(32);
var i64:int(64);
writeln( min( i8.type));
writeln( min( i16.type));
writeln( min( i32.type));
writeln( min( i64.type));
writeln( min( int(64)));

var ui8:uint(8);
var ui16:uint(16);
var ui32:uint(32);
var ui64:uint(64);
writeln( min( ui8.type));
writeln( min( ui16.type));
writeln( min( ui32.type));
writeln( min( ui64.type));
writeln( min( uint(64)));

var f32:real(32);
var f64:real;
writeln( min( f32.type));
writeln( min( f64.type));
writeln( min( real));

var c1:complex(64);
var c2:complex(128);
writeln( min( c1.type));
writeln( min( c2.type));
