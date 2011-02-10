/*
 * Linked List Example
 *
 * This example builds a linked list using an inner class for list nodes.
 * The List class defines a default head-to-tail iterator and a writeThis
 * method defining the way a list should be displayed when using the
 * write and writeln functions.
 *
 */

config const useClockSeed = false; // Use the clock to seed the RandomStream?
config const randomSeed = 1; // If not using the clock as a seed
config const listSize = 10;  // Used when populating the list

//
// A generic linked list class
//
class List {
  type eltType;
  var head: Node(eltType);

  //
  // The Node class contains the list's values and references to the
  // following node in the list. To avoid namespace pollution and
  // increase abstraction, it is defined within the List class.
  //
  class Node {
    type eltType;
    var value: eltType;
    var next: Node(eltType);
  }

  //
  // Insert value into the front of the list
  //
  proc insert(value: eltType) {
    head = new Node(eltType, value, head);
  }

  //
  // Return true if the list has any elements that match the argument in value
  //
  proc contains(value: eltType) {
    for i in this do
      if i == value then
        return true;
    return false;
  }

  //
  // Remove the first occurrence of an element that matches the argument value
  //
  proc remove(value: eltType) {
    if head == nil then
      return;

    if head.value == value {
      head = head.next;
      return;
    }

    var current = head;
    while current.next != nil {
      if current.next.value == value {
        current.next = current.next.next;
        return;
      }
      current = current.next;
    }
  }

  // 
  // A default iterator.  Starting at head, follow next references
  // and yield the values of each node.
  //
  iter these() {
    var current = head;
    while current != nil {
      yield current.value;
      current = current.next;
    }
  }
  //
  // Define the style of the output when a list is passed to the write or
  // writeln functions. The values will be written separated by spaces.
  // 
  proc writeThis(x: Writer) {
    var first = true;
    for i in this do
      if first {
        x.write(i);
        first = false;
      } else {
        x.write(" " + i);
      }
  }
}

proc main() {
  use Random;

  var lst = new List(int);
  var rnd = if useClockSeed 
              then new RandomStream()
              else new RandomStream(seed = randomSeed);
  const maxValue = 100;

  //
  // Add some integers between 1-100
  //
  for i in 1..listSize do
    lst.insert(1 + (maxValue * rnd.getNext()):int);
  //
  // writeln uses the writeThis method to display the list
  //
  writeln("The list contains: ", lst);

  //
  // Remove all ones from the list
  //
  if (lst.contains(1)) {
    writeln("The list includes a one");
    while(lst.contains(1)) do
      lst.remove(1);
    writeln("No more ones: ", lst);
  }

  //
  // Remove everything from the list using the default iterator
  // to find the values that need to be removed.
  //
  for i in lst do
    lst.remove(i);

  writeln("The list contains: ", lst);

  delete rnd;
}
