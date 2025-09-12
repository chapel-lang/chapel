

// DWARFDUMP: MyNode
class MyNode {
  var id: int;
  var next: unmanaged MyNode?;
  proc init(id: int) {
    this.id = id;
    this.next = nil;
  }
  proc deinit() {
    if this.next != nil {
      delete this.next;
    }
  }
}
// DWARFDUMP: myLinkedList
record myLinkedList {
  var head: unmanaged MyNode?;
  proc init() {
    this.head = nil;
  }
  proc deinit() {
    if this.head != nil {
      delete this.head;
    }
  }
  proc ref append(id: int) {
    if this.head == nil {
      this.head = new unmanaged MyNode(id);
    } else {
      var curr = this.head;
      while curr!.next != nil {
        curr = curr!.next;
      }
      curr!.next = new unmanaged MyNode(id);
    }
  }
  proc printList() {
    var curr = this.head;
    var sep = "";
    while curr != nil {
      write(sep, curr!.id);
      curr = curr!.next;
      sep = " ";
    }
    writeln();
  }
}

proc main() {
  // DWARFDUMP: thisIsTheList
  var thisIsTheList = new myLinkedList();
  thisIsTheList.append(1);
  thisIsTheList.append(2);
  thisIsTheList.append(3);
  thisIsTheList.printList();
  use Debugger; breakpoint;
}
