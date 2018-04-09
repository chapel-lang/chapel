class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo(arg:unmanaged Domain) {
    writeln("In parent foo");
  }
}
class MyArray : Array {
  var x;
}

proc test() {
  var arr = new unmanaged MyArray(1);
  var dom = new unmanaged MyDomain(1);
  arr.foo(dom);
}
test();
