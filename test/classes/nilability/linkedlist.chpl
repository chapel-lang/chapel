module linkedlist {
  class Node {
    var data: real;
    var next: owned Node?;
  }
  proc fnExpectingNotNil(x: Node) {
    writeln("fnExpectingNotNil data=", x.data);
  }

  var head = new Node(0);      // : borrowed Node - i.e. head cannot be nil
  var current: borrowed Node? = head; // Forget :Node? -> compiler error below on `current =` line
  for i in 1..numLocales-1 do on Locales[i] {
    current!.next = new owned Node(i);
    current = current!.next;
  }
  current = head;
  while current != nil {
    fnExpectingNotNil(current!);
    current = current!.next.borrow();
  }
}
