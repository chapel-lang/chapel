//===> Description ===>
//
// Definitions of classes Node and LinkedList for
// building simple linked lists.
//
//<=== Description <===



class Node {
  type element_type;
  var data: element_type;
  var next: Node(element_type);
}



class LinkedList {
  type element_type;
  var head, tail: Node(element_type);
  var is_empty = true;
}

def LinkedList.add_node (data: element_type) {
  var new_node = new Node(element_type);
  new_node.data = data;

  if is_empty then {
    head    = new_node;
    tail     = new_node;
    is_empty = false;
  }
  else {
    tail.next = new_node;
    tail      = new_node;
  }
}

def LinkedList.take_from_head() {
  if is_empty then
    return nil;
  else if head == tail {
    var return_node = head;
    clear();
    return return_node;
  }
  else {
    var return_node = head;
    head = head.next;
    return return_node;
  }
}

def LinkedList.take_from_tail() {

  if is_empty then
    return nil;

  else if head == tail {
    var return_node = tail;
    clear();
    return return_node;
  }

  else {
    var return_node = tail;

    var scanning_node = head;
    while scanning_node.next != tail do
      scanning_node = scanning_node.next;
    tail = scanning_node;
    tail.next = nil;

    return return_node;
  }
}

def LinkedList.clear() {
  head     = nil;
  tail     = nil;
  is_empty = true;
}




/* def main { */
/*   var list = new LinkedList(int); */
/*   var node = new Node(int); */

/*   for i in [1..10] do list.add_node(i); */

/*   while list.is_empty == false { */
/*     node = list.take_from_tail(); */
/*     writeln(node.data); */
/*   } */

/* } */
