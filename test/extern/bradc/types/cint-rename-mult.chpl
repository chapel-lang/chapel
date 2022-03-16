use CTypes;

extern "int" type c_int1 = c_int, c_int2 = c_int;

var x: c_int1;
var y: c_int2;

extern proc printf(x...);

printf("%d %d\n", x, y);

