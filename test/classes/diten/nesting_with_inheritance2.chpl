class intList {
  var head: unmanaged Node?;
  class Node {
    var value: int;
    var next: unmanaged Node?;
  }

  proc insert(value: int) {
    head = new unmanaged Node(value, head);
  }

  iter these() {
    var current = head;
    while current != nil {
      yield current.value;
      current = current.next;
    }
  }
}

class intSortedList: intList {
  proc insert(value: int) {
    if head == nil || head.value >= value {
      head = new unmanaged Node(value, head);
      return;
    }
    var current = head;
    while current.next != nil && current.next.value < value do
      current = current.next;
    current.next = new unmanaged Node(value, current.next);
  }
}

proc main() {
  var lst = new unmanaged intSortedList();

  delete lst;
}
