
var (x,(y,z)) = (1,(2,3));
var ((x2,y2),z2) = ((1,2),3);
var x3, y3, z3: int;
var (a,b,(c,(d,e))) = (1,2,(3,(4,5)));

((x3,y3),z3) = ((1,2),3);

writeln(x, " ", y, " ", z);
writeln(x2, " ", y2, " ", z2);
writeln(x3, " ", y3, " ", z3);
writeln(a, " ", b, " ", c, " ", d, " ", e);
