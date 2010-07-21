//===> Description ===>
//
// Definitions of classes Node and LinkedList for
// building simple linked lists.
//
//<=== Description <===




//===> Generic class Node ===>
//===========================>
class Node {
  type element_type;
  var data: element_type;
  var next: Node(element_type);
}
//<=== Generic class Node <===
//<===========================



//===> Generic class LinkedList ===>
//=================================>
class LinkedList {
  type element_type;
  var head, tail: Node(element_type);
  var is_empty = true;
}
//<=== Generic class LinkedList <===



//===> add_node method ===>
//========================>
def LinkedList.add_node (data: element_type) {
  var new_node = new Node(element_type);
  new_node.data = data;

  if is_empty then {
    head     = new_node;
    tail     = new_node;
    is_empty = false;
  }
  else {
    tail.next = new_node;
    tail      = new_node;
  }
}
//<========================
//<=== add_node_method <===




//===> remove_from_head() method ===>
//==================================>
def LinkedList.remove_from_head() {

  var data_out: element_type;

  if is_empty {
    data_out = nil;
  }
  else if head == tail {
    //==== Retrieve data ====
    data_out = head.data;

    //==== Clean ====
    clear();
  }
  else {
    //==== Retrieve data ====
    data_out = head.data;

    //==== Shift head and clean ====
    var old_head = head;
    head = head.next;
    delete old_head;
  }

  return data_out;
}
//<==================================
//<=== remove_from_head() method <===





//===> remove_from_tail() method ===>
//==================================>
def LinkedList.remove_from_tail() {

  var data_out: element_type;

  if is_empty {
    data_out = nil;
  }
  else if head == tail {
    //==== Retrieve data ====
    data_out = tail.data;

    //==== Clean ====
    clear();
  }
  else {
    //==== Retrieve data ====
    data_out = tail.data;

    //==== Shift tail and clean ====
    var old_tail = tail;

    var scan_node = head;
    while scan_node.next != tail do
      scan_node = scan_node.next;
    tail = scan_node;
    tail.next = nil;
    delete old_tail;
  }

  //==== Return ====
  return data_out;

}
//<=== remove_from_tail() method <===
//<==================================





//===> clear() method ===>
//=======================>
def LinkedList.clear() {
  //==== Delete all nodes ====
  var old_head = head;
  while old_head != nil {
    head = old_head.next;
    delete old_head;
    old_head = head;
  }

  head     = nil;
  tail     = nil;
  is_empty = true;
}
//<=======================
//<=== clear() method <===








/* class TestClass { */
/*   var value: int; */
/* } */


/* def main { */
/*   var list = new LinkedList(TestClass); */
/*   var data: TestClass; */

/*   for i in [1..10] { */
/*     data = new TestClass(value = i); */
/*     list.add_node(data); */
/*   } */


/*   while list.is_empty == false { */
/*     data = list.remove_from_tail(); */
/*     writeln(data.value); */
/*   } */

/*   writeln(list); */

/* } */
