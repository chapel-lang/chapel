/*
 *  Generic Stack Example
 *
 *  This example implements a generic stack data structure via an array
 *  and then instantiates the generic stack over strings and tuples of
 *  integers as examples of how to use the class.  This generic stack
 *  implementation is not thread-safe.
 *
 */

// A stack class that is generic over the type of data that it
// contains.  The implementation uses an array to store the elements
// in the stack.
record Stack {
  type itemType;            // type of items
  var numItems: int = 0;    // number of items in the stack
  var data: [1..2] itemType; // array of items

  // push method: add an item to the top of the stack
  // note: the array is doubled if it is full
  def push(item: itemType) {
    var height = data.numElements;
    if numItems == height then
      data.domain = [1..height*2];
    data(numItems+1) = item;
    numItems += 1;
  }

  // pop method: remove an item from the top of the stack
  // note: it is a runtime error if the stack is empty
  def pop() {
    if isEmpty then
      halt("attempt to pop an item off an empty stack");
    numItems -= 1;
    return data(numItems+1);
  }

  // isEmpty method: true if the stack is empty; otherwise false
  def isEmpty return numItems == 0;
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
