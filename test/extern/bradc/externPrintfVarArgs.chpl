_extern def printf(fmt: string, vals...?numvals): int;

var x = 12.34;

printf("%e %f %g %14.14f\n", x, x, x, x);
