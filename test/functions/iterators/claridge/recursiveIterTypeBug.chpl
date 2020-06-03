//----------------------------------------------------------------
// This will compile and run without errors if either:
// (1) The two lines of code involving 'node' are uncommented;
// (2) Rather than calling yieldInorder(list.head), first declare
//         var head = list.head;
//     and then call yieldInorder(head).
//
// Also, the non-recursive version of the iterator, commented out
// at the bottom, works just fine.
//----------------------------------------------------------------


proc main {

  // var node: unmanaged Node;
  // for n in traverse(node) do writeln(n.data);

  var list = new unmanaged List("A");

  list.head.next = new unmanaged Node("B");

  for n in traverse(list.head) do writeln(n.data);

  delete list.head.next;
  delete list;
}



class List {
  var head: unmanaged Node;

  proc init ( str: string ) { head = new unmanaged Node(str); }

  proc deinit() { delete head; }
}


class Node {
  var data: string;
  var next: unmanaged Node?;
}


iter traverse( node: unmanaged Node? ) : unmanaged Node
{
  if node != nil {
    yield node!;
    for child in traverse(node!.next) do yield child;
  }
}


//---- The non-recursive version works just fine. ----
//
// iter traverse( node: unmanaged Node ) : unmanaged Node
// {
//   var n = node;
//   while n != nil {
//     yield n;
//     n = n.next;
//   }
// }
