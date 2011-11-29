class intList {
  var head: Node;
  class Node {
    var value: int;
    var next: Node;
  }

  proc insert(value: int) {
    head = new Node(value, head);
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
      head = new Node(value, head);
      return;
    }
    var current = head;
    while current.next != nil && current.next.value < value do
      current = current.next;
    current.next = new Node(value, current.next);
  }
}

proc main() {
  var lst = new intSortedList();
}
