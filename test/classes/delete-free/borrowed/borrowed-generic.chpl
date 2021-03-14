class Node {
  type t;
  var data:t;
  var next: borrowed Node(t)?;
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
  var nodes:[1..n] unmanaged Node(int) =
    for i in 1..n do new unmanaged Node(i);

  var head:borrowed Node(int) = nodes[1];
  var current: borrowed Node(int)? = head;
  for i in 1..n-1 {
    current!.next = nodes[i+1];
    current       = current!.next;
  }

  current = head;
  while current {
    var ptr = current!;
    current = current!.next;
    writeln(ptr.data);
  }
  
  for i in 1..n {
    delete nodes[i];
  }
}

test1();
