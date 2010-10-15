class Node {
  type data_type;
  var data: data_type;
  var next: Node(data_type);
}

record Stack {
  type data_type;
  var top: Node(data_type);
  
  def push(data: data_type) {
    top = new Node(data_type, data, top);
  }
  
  def pop() {
    if isEmpty() then 
      halt("Attempting to pop off an empty stack.");
    var old_top = top;
    top = old_top.next;
    return old_top.data;
  }
  
  def isEmpty() {
    return top==nil;
  }
}



def main {
  var int_stack = new Stack(int);
  int_stack.push(1);
  int_stack.push(2);
  int_stack.push(3);
  
  while !int_stack.isEmpty() {
    var i = int_stack.pop();
    writeln(i);
  }
  writeln(int_stack.isEmpty());
}