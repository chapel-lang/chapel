class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo(arg: unmanaged Domain) {
    writeln("In parent foo");
  }
  proc bar(arg: borrowed Domain) {
    writeln("In parent bar");
  }
}
class MyArray : Array {
  var x;
}

proc baz(arg: borrowed Domain) {
  writeln("In bar arg:Domain");
}

proc test() {
  var arr = new unmanaged MyArray(1);
  var dom = new unmanaged MyDomain(1);
  arr.foo(dom);
  arr.bar(dom);
  baz(dom);
  delete dom;
  delete arr;
}
test();
