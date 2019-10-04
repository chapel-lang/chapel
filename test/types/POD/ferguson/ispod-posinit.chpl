proc doit(type t) {
  writeln(t:string, " : ", isPODType(t));
}

record NotPod1 {
  var x:int;
  proc postinit() {
    writeln("in postinit");
  }
}

doit(NotPod1);
