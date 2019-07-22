class Node {
  type t;
  var data:t;
  var next: unmanaged Node(t)?;
  proc init(type t) {
    this.t = t;
    this.next = nil;
  }
  proc init(arg) {
    this.t = arg.type;
    this.data = arg;
    this.next = nil;
  }
}

config const n = 5;

proc test1() {
  var head    = new unmanaged Node(0);
  var current: unmanaged Node(int)? = head;
  for i in 1..n-1 {
    current!.next = new unmanaged Node(i);
    current       = current!.next;
  }

  current = head;
  while current {
    var ptr = current!;
    current = current!.next;
    writeln(ptr.data);
    delete ptr;
  }
}

test1();

class MyClass {
  type T;
  var x : T;
}

proc makeit(type t) {
  return new t(1);
}

proc test2() {
  var x = makeit(unmanaged MyClass(int));
  writeln(x);
  delete x;
}

test2();

