var Vector: domain(1);
var a, b, c: [Vector] real;

Vector = {1..5};
a[Vector] = 1.2;
a[3] = 9.3;
writeln( a);
Vector = {1..10};
writeln( a);

