proc main {
  
  writeln("List test:");
  var l = new List(int);
  l.add(1);
  l.add(2);
  l.add(3);
  write("List contents:");
  for i in l do write(" ", i);
  writeln("");
  
  l.clear();
  write("List contents after clearing:");
  for i in l do write(" ", i);
  writeln("");
  
  
  writeln("");
  writeln("Stack test:");
  var s = new Stack(int);
  s.push(1);
  s.push(2);
  s.push(3);
  while !s.isEmpty() do writeln(s.pop());
  
  
  writeln("\nQueue test:");
  var q = new Queue(string);
  q.enqueue("a");
  q.enqueue("b");
  q.enqueue("c");
  while !q.isEmpty() do writeln(q.dequeue());
  
}






//|\"""""""""""""|\
//| >    List    | >
//|/_____________|/

record List
{
  
  type data_type;
  var head: Node;
  
  
  class Node 
  {
    var data: outer.data_type;
    var next: Node;
  }
  
  
  iter these ()
  {
    var node = head;
    while node { yield node.data; node = node.next; }
  }
  
  
  proc add ( data: data_type )
  {
    // This should work even if head==nil.
    
    head = new Node( data, head );
  }
  
  
  proc clear ()
  {
    var next_node: Node;
    
    while head {
      next_node = head.next;
      delete head;
      head = next_node;
    }

  }
  
  
  proc isEmpty () { return head==nil; }
  
  
}

// /|"""""""""""""/|
//< |    List    < |
// \|_____________\|





//|\""""""""""""""|\
//| >    Stack    | >
//|/______________|/

record Stack
{

  type data_type;
  var top:  Node;
  var size: int=0;

  
  class Node {
    var data: outer.data_type;
    var next: Node;
  }

  
  proc push ( data: data_type )
  {
    top = new Node(data, top);
    size += 1;
  }

  
  proc pop ()
  {
    if isEmpty() then halt("Attempting to pop off an empty stack.");
    
    var old_top = top;
    top = old_top.next;
    size -= 1;

    return old_top.data;
  }

  
  proc isEmpty() { return top==nil; }

}

// /|""""""""""""""/|
//< |    Stack    < |
// \|______________\|






//|\""""""""""""""|\
//| >    Queue    | >
//|/______________|/

record Queue
{
  
  type data_type;
  var head: Node;
  var tail: Node;

  class Node {
    var data: outer.data_type;
    var prev: Node;
    var next: Node;
  }



  proc enqueue (data: data_type)
  {
    if tail {
      var old_tail = tail;
      tail = new Node(data);
      old_tail.next = tail;
      tail.prev     = old_tail;
    }
    else {
      head = new Node(data);
      tail = head;
    }
  }


  proc dequeue ()
  {
  
    if head==nil then
      halt("Error: Attempted to dequeue from empty Queue.");
    else
    {
      var output_data = head.data;
    
      if head == tail {
        delete head;
        head = nil;
        tail = nil;
      }
      else {
        var old_head = head;
        head = head.next;
        delete old_head;
      }
    
      return output_data;
    }
  
  }


  proc clear ()
  {
    var node = head;
  
    while node
    {
      var next_node = node.next;
      delete node;
      node = next_node;
    }
  }


  proc isEmpty()
  {
    if head then return false;
    else return true;
  }

}

// /|""""""""""""""/|
//< |    Queue    < |
// \|______________\|
