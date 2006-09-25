// Test the min(type) primitive.

var i8:int(8);
var i16:int(16);
var i32:int(32);
var i64:int;
writeln( min( i8.type));
writeln( min( i16.type));
writeln( min( i32.type));
writeln( min( i64.type));
writeln( min( int));

var ui8:uint(8);
var ui16:uint(16);
var ui32:uint(32);
var ui64:uint;
writeln( min( ui8.type));
writeln( min( ui16.type));
writeln( min( ui32.type));
writeln( min( ui64.type));
writeln( min( uint));

var f32:float(32);
var f64:float;
writeln( min( f32.type));
writeln( min( f64.type));
writeln( min( float));
