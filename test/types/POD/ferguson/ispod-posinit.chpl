proc doit(type t) {
  writeln(t:string, " : ", __primitive("is pod type", t));
}

record NotPod1 {
  var x:int;
  proc postinit() {
    writeln("in postinit");
  }
}

doit(NotPod1);
