class intList {
  var head: Node;
  class Node {
    var value: int;
    var next: Node;
  }

  def insert(value: int) {
    head = Node(value, head);
  }

  def these() {
    var current = head;
    while current != nil {
      yield current.value;
      current = current.next;
    }
  }
}

class intSortedList: intList {
  def insert(value: int) {
    if head == nil || head.value >= value {
      head = Node(value, head);
      return;
    }
    var current = head;
    while current.next != nil && current.next.value < value do
      current = current.next;
    current.next = Node(value, current.next);
  }
}

def main() {
  var lst = intSortedList();
}
