class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo(arg:raw Domain) {
    writeln("In parent foo");
  }
}
class MyArray : Array {
  var x;
}

proc test() {
  var arr = new raw MyArray(1);
  var dom = new raw MyDomain(1);
  arr.foo(dom);
}
test();
