extern proc printf(fmt: c_string, vals...?numvals): int;

var x = 12.34;

printf("%e %f %g %14.14f\n", x, x, x, x);
