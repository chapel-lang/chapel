proc doit(type t) {
  writeln(t:string, " : ", __primitive("is pod type", t));
}

record NotPod1 {
  var x:int;
}

proc =(ref lhs:NotPod1, rhs:NotPod1) {
  writeln("in assign");
}

doit(NotPod1);
