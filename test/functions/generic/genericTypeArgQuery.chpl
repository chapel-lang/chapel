proc foo(type t: C(?et)) {
  writeln(" t is: ", t:string);
  writeln("et is: ", et: string);
}

class C {
  type eltType;
  var x: eltType;
}

foo(C(int));
foo(C(real));
