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
  var head: unmanaged Node(eltType)?;

  //
  // The Node class contains the list's values and references to the
  // following node in the list. To avoid namespace pollution and
  // increase abstraction, it is defined within the List class.
  //
  class Node {
    type eltType;
    var value: eltType;
    var next: unmanaged Node(eltType)?;
  }

  //
  // Insert value into the front of the list
  //
  proc insert(value: eltType) {
    head = new unmanaged Node(eltType, value, head);
  }
 
  
 //  
 // deletes the first element from the list 
 //

 proc delete_first() {
    if head == nil then {
      return;
    }
    var current=head!;
    head=head!.next;
    delete current ;
  }

 //
 // deletes the element at position p from the list 
 //
 
 proc delete_pos(pos: int) {
    if head==nil then {
      return;
    }


    if pos==1 then {
      delete_first();
      return; 
    }

    var count: int=1;   // assuming 1-based indexing //
    var current=head;
    

    while(count!=pos-1 && current!=nil) {  
     current=current!.next;
     count=count+1;
    }
   
    if current!.next==nil then  { 
     return;
    }
  
    var temp=current!.next!;
    var temp2=temp.next;
    current!.next=temp2;
    delete temp;
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

    if head!.value == value {
      const match = head!;
      head = head!.next;
      delete match;
      return;
    }

    var current = head!;
    while current.next != nil {
      if current.next!.value == value {
        const match = current.next!;
        current.next = current.next!.next;
        delete match;
        return;
      }
      current = current.next!;
    }
  }

  // 
  // A default iterator.  Starting at head, follow next references
  // and yield the values of each node.
  //
  iter these() {
    var current = head;
    while current != nil {
      const next = current!.next;
      yield current!.value;
      current = next;
    }
  }
  //
  // Define the style of the output when a list is passed to the write or
  // writeln functions. The values will be written separated by spaces.
  // The argument 'w' is a writeable channel.
  proc writeThis(w) throws {
    var first = true;
    for i in this do
      if first {
        w.write(i);
        first = false;
      } else {
        w.write(" ", i);
      }
  }
}

proc main() {
  use Random;

  var lst = new owned List(int);
  var rnd = if useClockSeed 
              then new owned NPBRandomStream(real)
              else new owned NPBRandomStream(real, seed = randomSeed);
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


  lst.delete_first();
 writeln("The list after deleting first contains: ", lst);


 lst.delete_pos(3);
writeln("delete element at pos 3 :", lst);
 lst.delete_pos(1);
writeln("delete element at pos 1 :", lst);
 lst.delete_pos(5);
 writeln("delete element at pos 5 :", lst);

 
 
  
  //
  // Remove everything from the list using the default iterator
  // to find the values that need to be removed.
  //
  for i in lst do
    lst.remove(i);

  writeln("The list contains: ", lst);
}
