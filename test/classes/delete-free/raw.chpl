class Node {
  var data: real;
  var next: raw Node;
  proc init(arg:real) {
    this.data = arg;
    this.next = nil;
  }
}

config const n = 5;

proc test1() {
  var head    = new raw Node(0);
  var current = head;
  for i in 1..n-1 {
    current.next = new raw Node(i);
    current      = current.next;
  }

  current = head;
  while current {
    var ptr = current;
    current = current.next;
    writeln(ptr.data);
    delete ptr;
  }
}

test1();

class MyClass {
  var x:int;
}

proc makeit(type t) {
  return new t(1);
}

proc test2() {
  var x = makeit(raw MyClass);
  writeln(x);
}

test2();
