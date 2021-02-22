proc doit(type t) {
  writeln(t:string, " : ", isPODType(t));
}

record NotPod1 {
  var x:int;
}

operator =(ref lhs:NotPod1, rhs:NotPod1) {
  writeln("in assign");
}

doit(NotPod1);
