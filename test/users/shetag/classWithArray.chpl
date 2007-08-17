class C {
  const arr : [1..2] int;
}

const a : [1..2] int = (1, 2);

var c2 = C(a); //compiles correctly

var c1 : C;
c1 = C(a); //type mismatch in assignment from C([domain(1,int,0)] int) to C(nil);

writeln("c1 is: ", c1);
writeln("c2 is: ", c2);
