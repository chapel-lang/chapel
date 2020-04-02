/*
 *  Generic Stack Examples
 *
 *  This example implements a generic stack data structure using two
 *  methods: (1) via a linked list; (2) via an array.  It then
 *  instantiates the generic stacks over strings and tuples of
 *  integers as examples of how to use the class.  These generic stack
 *  implementations are not thread-safe.
 *
 */

// A stack class that is generic over the type of data that it
// contains.  This implementation uses a linked list implemented using
// the node class contained within it.
record ListStack {
  // A class that is used by the generic stack to implement nodes for a
  // linked-list implementation.
  class MyNode {
    type itemType;              // type of item
    var item: itemType;         // item in node
    var next: unmanaged MyNode(itemType)?; // reference to next node (same type)
  }

  type itemType;             // type of items
  var top: unmanaged MyNode(itemType)?; // top node on stack linked list

  // push method: add an item to the top of the stack
  proc push(item: itemType) {
    top = new unmanaged MyNode(itemType, item, top);
  }

  // pop method: remove an item from the top of the stack
  // note: it is a runtime error if the stack is empty
  proc pop() {
    if isEmpty then
      halt("attempt to pop an item off an empty stack");
    var oldTop = top;
    var oldItem = top!.item;
    top = top!.next;
    delete oldTop;
    return oldItem;
  }

  // isEmpty method: true if the stack is empty; otherwise false
  proc isEmpty return top == nil;
}


// A stack class that is generic over the type of data that it
// contains.  This implementation uses an array to store the elements
// in the stack.
record ArrayStack {
  type itemType;            // type of items
  var numItems: int = 0;    // number of items in the stack
  var dataSpace: domain(1) = {1..2};
  var data: [dataSpace] itemType; // array of items

  // push method: add an item to the top of the stack
  // note: the array is doubled if it is full
  proc push(item: itemType) {
    var height = data.size;
    if numItems == height then
      dataSpace = {1..height*2};
    data(numItems+1) = item;
    numItems += 1;
  }

  // pop method: remove an item from the top of the stack
  // note: it is a runtime error if the stack is empty
  proc pop() {
    if isEmpty then
      halt("attempt to pop an item off an empty stack");
    numItems -= 1;
    return data(numItems+1);
  }

  // isEmpty method: true if the stack is empty; otherwise false
  proc isEmpty return numItems == 0;
}




var D: domain(2) = {1..4, 1..4};
var s1 = new ListStack(string);
var s2 = new ListStack(index(D));
var s3 = new ArrayStack(string);
var s4 = new ArrayStack(index(D));
testStacks(s1, s2);
testStacks(s3, s4);


proc testStacks(ref stack1, ref stack2) {
  // Test: Push three strings onto a stack and then pop them off.
  stack1.push("one");
  stack1.push("two");
  stack1.push("three");
  writeln(stack1.pop());
  writeln(stack1.pop());
  writeln(stack1.pop());

  // Test: Iterate over a domain, pushing the indices (tuples of
  // integers) onto a stack, and then pop them all off.
  for i in D do
    stack2.push(i);
  while !stack2.isEmpty do
    writeln(stack2.pop());
}
