
/*****
      ex_tuple.chpl -
      Examples of tuple usage.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

writeln("\nDemonstrating tuples");

var coord3D_1 : (int, int, int);
var coord3D_2 : 3*int;
var coord1D_1 : 1*int;
var coord3D_3 = (10, 20, 30);
var coord1D_2 = (100, );

writeln("y = ", coord3D_3(1));

writef("\n");
for x in coord3D_3 do writeln("coord = ", x);
for i in 0..(coord3D_3.size-1) do writeln("coord ", i, " = ", coord3D_3(i));

writef("\n");

var x, y, z : int;

(x, _, z) = coord3D_3;
writeln("x = ", x, "  y = ", y, "  z = ", z);

writef("\n");
coord3D_2 = -coord3D_3;
for x in coord3D_2 do writeln("- coord = ", x);

coord3D_1 = coord3D_3 * coord3D_3;
for x in coord3D_1 do writeln("coord*coord = ", x);

writef("\n");
writeln("coord3D_3 == -coord3D_2 ? ", coord3D_3 == -coord3D_2);
writeln("(1,1,1,0,1,1,1) != (1,1,1,1,1,1,1) ? ",
        (1,1,1,0,1,1,1) != (1,1,1,1,1,1,1));

writef("\n");
writeln("fail 1st index: (1, 2, 3) >  (2, 2, 2) ? ", (1, 2, 3) > (2, 2, 2));
writeln("pass 1st index: (3, 2, 1) >  (2, 2, 2) ? ", (3, 2, 1) > (2, 2, 2));
writeln("fail 2nd index: (2, 1, 3) >  (2, 2, 2) ? ", (2, 1, 3) > (2, 2, 2));
writeln("pass 2nd index: (2, 3, 1) >  (2, 2, 2) ? ", (2, 3, 1) > (2, 2, 2));
writeln("fail 3rd index: (2, 2, 1) >  (2, 2, 2) ? ", (2, 2, 1) > (2, 2, 2));
writeln("pass 3rd index: (2, 2, 3) >  (2, 2, 2) ? ", (2, 2, 3) > (2, 2, 2));
writeln("fail at end:    (2, 2, 2) >  (2, 2, 2) ? ", (2, 2, 2) > (2, 2, 2));
writeln("pass at end:    (2, 2, 2) >= (2, 2, 2) ? ", (2, 2, 2) >= (2, 2, 2));
