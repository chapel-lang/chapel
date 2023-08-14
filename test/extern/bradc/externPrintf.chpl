//_extern proc printf(fmt: c_string, vals...?numvals): int;
use CTypes;
extern proc printf(fmt: c_ptrConst(c_char), val1: real, val2:real, val3:real, val4:real): int;

var x = 12.34;

printf("%e %f %g %14.14f\n", x, x, x, x);
