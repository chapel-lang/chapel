proc main {
  
  writeln("List test:");
  var l = new unmanaged List(int);
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
  var s = new unmanaged Stack(int);
  s.push(1);
  s.push(2);
  s.push(3);
  while !s.isEmpty() do writeln(s.pop());
  
  
  writeln("\nQueue test:");
  var q = new unmanaged Queue(string);
  q.enqueue("a");
  q.enqueue("b");
  q.enqueue("c");
  while !q.isEmpty() do writeln(q.dequeue());
  delete q;
}






//|\"""""""""""""|\
//| >    List    | >
//|/_____________|/

class List
{
  
  type data_type;
  var head: unmanaged Node(data_type)?;
  
  
  class Node 
  {
    type data_type;
    var data: data_type;
    var next: unmanaged Node(data_type)?;
  }
  
  
  proc deinit () { clear(); }
  
  
  iter these ()
  {
    var node = head;
    while node { yield node!.data; node = node!.next; }
  }
  
  
  proc add ( data: data_type )
  {
    // This should work even if head==nil.
    
    head = new unmanaged Node(data_type, data, head );
  }
  
  
  proc clear ()
  {
    var next_node: unmanaged Node(data_type)?;
    
    while head {
      next_node = head!.next;
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

class Stack
{

  type data_type;
  var top:  unmanaged Node(data_type)?;

  
  class Node {
    type data_type;
    var data: data_type;
    var next: unmanaged Node(data_type)?;
  }


  proc deinit ()
  {
    while top do pop();
  }
  
  
  proc push ( data: data_type )
  {
    top = new unmanaged Node(data_type, data, top);
  }

  
  proc pop ()
  {
    if isEmpty() then halt("Attempting to pop off an empty stack.");
    
    var data_out = top!.data;
    var new_top  = top!.next;
    delete top;
    top = new_top;

    return data_out;

  }

  
  proc isEmpty() { return top==nil; }

}

// /|""""""""""""""/|
//< |    Stack    < |
// \|______________\|






//|\""""""""""""""|\
//| >    Queue    | >
//|/______________|/

class Queue
{
  
  type data_type;
  var head: unmanaged Node(data_type)?;
  var tail: unmanaged Node(data_type)?;

  class Node {
    type data_type;
    var data: data_type;
    var prev: unmanaged Node(data_type)?;
    var next: unmanaged Node(data_type)?;
  }



  proc deinit ()
  {
    while head do dequeue();
  }



  proc enqueue (data: data_type)
  {
    if tail {
      var old_tail = tail!;
      tail = new unmanaged Node(data_type, data);
      old_tail.next = tail;
      tail!.prev    = old_tail;
    }
    else {
      head = new unmanaged Node(data_type, data);
      tail = head;
    }
  }


  proc dequeue ()
  {
  
    if head==nil then
      halt("Error: Attempted to dequeue from empty Queue.");
    else
    {
      var output_data = head!.data;
    
      if head == tail {
        delete head;
        head = nil;
        tail = nil;
      }
      else {
        var old_head = head;
        head = head!.next;
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
