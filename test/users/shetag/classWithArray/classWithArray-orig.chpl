class C {
  const arr : [1..2] int;
}

const a : [1..2] int;

var c2 = new C(a); //compiles correctly

var c1 : C;
c1 = new C(a); //type mismatch in assignment from C([domain(1,int,0)] int) to C(nil);
