proc doit(type t) {
  writeln(typeToString(t), " : ", __primitive("is pod type", t));
}

class MyClass {
  var x:atomic int;
}

record NotPod1 {
  var x:int;
  proc ~NotPod1() { writeln("custom destructor"); }
}

record NotPod2 {
  var x:int;
}

pragma "auto copy fn" proc chpl__autoCopy(x:NotPod2) {
  writeln("custom auto copy");
  return x;
}

record NotPod3 {
  var x:int;
}

proc chpl__autoDestroy(x:NotPod3) {
  writeln("custom auto destroy");
  return x;
}

pragma "ignore noinit"
record NotPod4 {
  var x:int;
}

record NotPod5 {
  var x:NotPod2;
}

record NotPod6 {
  var x:NotPod5;
}

record NotPod7 {
  var x:NotPod6;
}

record Pod1 {
  var x:int;
  var y:int;
}

record Pod2 {
  var x:real;
  var y:real;
}

record Pod3 {
  var x:complex;
  var y:int;
}

record ExternStruct {
  var x:c_int;
};

extern type ExternOpaque;

doit(bool);
doit(int(16));
doit(int);
doit(complex);
doit(c_int);
doit(MyClass);
doit(atomic int);
doit(NotPod1);
doit(NotPod2);
doit(NotPod3);
doit(NotPod4);
doit(NotPod5);
doit(NotPod6);
doit(NotPod7);
doit(Pod1);
doit(Pod2);
doit(Pod3);
doit(ExternStruct);
doit(ExternOpaque);

