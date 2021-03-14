proc foo(type t: borrowed C(?et)) {
  writeln(" t is: ", t:string);
  writeln("et is: ", et: string);
}

class C {
  type eltType;
  var x: eltType;
}

foo(borrowed C(int));
foo(borrowed C(real));
