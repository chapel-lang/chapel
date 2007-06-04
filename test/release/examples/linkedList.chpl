config const n = 5;

class List {
  type eltType;
  var head: Node(eltType);
  var tail: Node(eltType);

  class Node {
    type eltType;
    var val: eltType;
    var next: Node(eltType);
    var prev: Node(eltType);
  }

  def insertAtHead(value: eltType) {
    if head == nil {
      head = Node(eltType, value);
      tail = head;
    } else {
      var newHead = Node(eltType, value, head);
      head.prev = newHead;
      head = newHead;
    }
  }

  def insertAtTail(value: eltType) {
    if head == nil {
      head = Node(eltType, value);
      tail = head;
    } else {
      var newTail = Node(eltType, value, nil, tail);
      tail.next = newTail;
      tail = newTail;
    }
  }

  iterator ault() {
    var current = head;
    while current != nil {
      yield current.val;
      current = current.next;
    }
  }

  iterator backward() {
    var current = tail;
    while current != nil {
      yield current.val;
      current = current.prev;
    }
  }
}


def main() {
  var lst = List(real);
  var lst2= List(int);
  for i in 1..n {
    lst.insertAtHead(i*2.0);
    lst2.insertAtHead(i*2);
  }

  for (i,j) in (lst, lst2) do
    writeln(i, " ", j);

  for (i,j) in (lst.backward(), lst2.backward()) do
    writeln(i, " ", j);
}

