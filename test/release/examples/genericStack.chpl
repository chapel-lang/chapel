/*
 *  Generic Stack Example
 *
 *  This example implements a generic stack data structure via a
 *  linked list and then instantiates the generic stack over strings
 *  and tuples of integers as examples of how to use the class.  This
 *  generic stack implementation is not thread-safe.
 *
 */

// A class that is used by the generic stack to implement nodes for a
// linked-list implementation.
class MyNode {
  type itemType;              // type of item
  var item: itemType;         // item in node
  var next: MyNode(itemType); // reference to next node (same type)
}

// A stack class that is generic over the type of data that it
// contains.  The implementation uses a linked list implemented with
// the node class above.
record Stack {
  type itemType;             // type of items
  var top: MyNode(itemType); // top node on stack linked list

  // push method: add an item to the top of the stack
  def push(item: itemType) {
    top = MyNode(itemType, item, top);
  }

  // pop method: remove an item from the top of the stack
  // note: it is a runtime error if the stack is empty
  def pop() {
    if isEmpty then
      halt("attempt to pop an item off an empty stack");
    var oldTop = top;
    top = top.next;
    return oldTop.item;
  }

  // isEmpty method: true if the stack is empty; otherwise false
  def isEmpty return top == nil;
}

// Test: Push three strings onto a stack and then pop them off.
var stack1: Stack(string);
stack1.push("one");
stack1.push("two");
stack1.push("three");
writeln(stack1.pop());
writeln(stack1.pop());
writeln(stack1.pop());

// Test: Iterate over a domain, pushing the indices (tuples of
// integers) onto a stack, and then pop them all off.
var D: domain(2) = [1..4, 1..4];
var stack2: Stack(index(D));
for i in D do
  stack2.push(i);
while !stack2.isEmpty do
  writeln(stack2.pop());
