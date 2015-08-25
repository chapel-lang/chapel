proc doit(type t) {
  writeln(typeToString(t), " : ", __primitive("is pod type", t));
}

record NotPod1 {
  var x:int;
  proc initialize() {
    writeln("in initialize");
  }
}

doit(NotPod1);
