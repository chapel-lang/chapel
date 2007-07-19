module dequeue {

  record DeQueue {

    class MyNode {
      type itemType;              // type of item
      var item: itemType;         // item in node
      var prev: MyNode(itemType); // reference to prev node (same type)
      var next: MyNode(itemType); // reference to next node (same type)
    }

    type itemType;             // type of items
    var size: int;             // Length of the DeQueue
    var top: MyNode(itemType); // top node on DeQueue linked list
    var bottom: MyNode(itemType);
    var id: int;

    // pushTop: add an item to the top of the DeQueue
    def pushTop(item: itemType) {
      var newTop = MyNode(itemType, item, nil, top);
      if top == nil {
        bottom = newTop;
      } else {
        top.prev = newTop;
      }
      top = newTop;
      size += 1;
    }

    // pushBottom: add an item to the bottom of the DeQueue
    def pushBottom(item: itemType) {
      var newBottom = MyNode(itemType, item, bottom, nil);
      if bottom == nil {
        top = newBottom;
      } else {
        bottom.next = newBottom;
      }
      bottom = newBottom;
      size += 1;
    }
    
    // popTop: remove an item from the top of the DeQueue
    // note: it is a runtime error if the stack is empty
    def popTop() {
      if isEmpty() then
        halt("attempt to pop an item off an empty DeQueue");
      var oldTop = top;
      top = top.next;
      if top == nil then bottom = nil;  // Empty
      size -= 1;
      return oldTop.item;
    }

    // popBottom: remove an item from the bottom of the DeQueue
    def popBottom() {
      if isEmpty() then
        halt("attempt to pop an item off an empty DeQueue");
      var oldBottom = bottom;
      bottom = bottom.prev;
      if bottom == nil then top = nil;  // Empty
      size -= 1;
      return oldBottom.item;
    }

    // Split n elements off the bottom of the Dequeue and
    // return as a new stack.
    def split(n: int): DeQueue(itemType) {
      if (n > size) then
        halt("attempt to split more items than are in DeQueue");

      var newTop = bottom;
      var newBottom = bottom;

      for i in [1..n-1] {
        newTop = newTop.prev;
      }

      // Remove from the current stack
      bottom = newTop.prev;
      bottom.next = nil;
      size -= n;

      // Update new top
      newTop.prev = nil;

      return DeQueue(itemType, n, newTop, newBottom, id+1);
    }

    // isEmpty: true if the stack is empty; otherwise false
    def isEmpty() {
      return top == nil;
    }
  }
}
