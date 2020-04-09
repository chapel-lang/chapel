module dequeue {

  record DeQueue {

    class MyNode {
      type itemType;              // type of item
      var item: itemType;         // item in node
      var prev: unmanaged MyNode(itemType)?; // reference to prev node (same type)
      var next: unmanaged MyNode(itemType)?; // reference to next node (same type)
    }

    type itemType;             // type of items
    var size: int;             // Length of the DeQueue
    var top: unmanaged MyNode(itemType)?; // top node on DeQueue linked list
    var bottom: unmanaged MyNode(itemType)?;
    var id: int;

    // pushTop: add an item to the top of the DeQueue
    proc pushTop(item: itemType) {
      var newTop = new unmanaged MyNode(itemType, item, nil, top);
      if top == nil {
        bottom = newTop;
      } else {
        top!.prev = newTop;
      }
      top = newTop;
      size += 1;
    }

    // pushBottom: add an item to the bottom of the DeQueue
    proc pushBottom(item: itemType) {
      var newBottom = new unmanaged MyNode(itemType, item, bottom, nil);
      if bottom == nil {
        top = newBottom;
      } else {
        bottom!.next = newBottom;
      }
      bottom = newBottom;
      size += 1;
    }
    
    // popTop: remove an item from the top of the DeQueue
    // note: it is a runtime error if the stack is empty
    proc popTop() {
      if isEmpty() then
        halt("attempt to pop an item off an empty DeQueue");
      var oldTop = top;
      top = top!.next;
      if top == nil then bottom = nil;  // Empty
      size -= 1;
      var result = oldTop!.item;
      delete oldTop;
      return result;
    }

    // popBottom: remove an item from the bottom of the DeQueue
    proc popBottom() {
      if isEmpty() then
        halt("attempt to pop an item off an empty DeQueue");
      var oldBottom = bottom;
      bottom = bottom!.prev;
      if bottom == nil then top = nil;  // Empty
      size -= 1;
      var result = oldBottom!.item;
      delete oldBottom;
      return result;
    }

    // Split n elements off the bottom of the Dequeue and
    // return as a new stack.
    proc split(n: int): DeQueue(itemType) {
      if (n > size) then
        halt("attempt to split more items than are in DeQueue");

      var newTop = bottom;
      var newBottom = bottom;

      for i in 1..n-1 {
        newTop = newTop!.prev;
      }

      // Remove from the current stack
      bottom = newTop!.prev;
      bottom!.next = nil;
      size -= n;

      // Update new top
      newTop!.prev = nil;

      return new DeQueue(itemType, n, newTop, newBottom, id+1);
    }

    // isEmpty: true if the stack is empty; otherwise false
    proc isEmpty() {
      return top == nil;
    }
  }

    proc main() {
      { // was deq_testTop.chpl
        var q = new DeQueue(int);

        for i in 1..10 do
          q.pushTop(i);
        
        for i in 1..10 do
          writeln(q.popTop(), ", size=", q.size);

        writeln("--");
        
        for i in 1..10 do
          q.pushTop(i);
        
        for i in 1..10 do
          writeln(q.popBottom(), ", size=", q.size);
        
        writeln("--");
        
        for i in 1..10 do
          q.pushTop(i);
        
        var q2 = q.split(5);
        
        for i in 1..5 do
          writeln(q.popTop(), " ", q2.popTop());
        
      }
      { // was deq_testBottom.chpl
        var q = new DeQueue(int);

        for i in 1..10 do
          q.pushBottom(i);

        for i in 1..10 do
          writeln(q.popTop(), ", size=", q.size);

        writeln("--");

        for i in 1..10 do
          q.pushBottom(i);

        for i in 1..10 do
          writeln(q.popBottom(), ", size=", q.size);

        writeln("--");

        for i in 1..10 do
          q.pushBottom(i);

        var q2 = q.split(5);

        for i in 1..5 do
          writeln(q.popTop(), " ", q2.popTop());
      }
    }
}
