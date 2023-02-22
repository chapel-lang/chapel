class ListNode {
  var next: shared ListNode?;
  var x: int;

  proc init(node: shared ListNode?, val: int) {
    next = node;
    x = val;
  }

  proc deinit() {
    writeln("Cleaning up x=", x);
  }
}

class List {
  var head: ListNode?;

  proc deinit() {
    while head != nil {
      var headTmp = head;
      head = head!.next;
      headTmp!.next = nil;
    }
  }
}

proc main() {
  var list = new List(new shared ListNode?(nil: shared ListNode?, 3));
  (list.head)!.next = new shared ListNode?(new shared ListNode?(nil: shared ListNode?, 1), 2);
  writeln(list);
}
