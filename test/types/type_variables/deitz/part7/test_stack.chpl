class stack_elt {
  type eltType;
  var value : eltType;
  var next : stack_elt(eltType);
}

record stack {
  type eltType;
  var top : stack_elt(eltType);
}

def stack.empty
  return top == nil;

def stack.push(v : eltType) {
  top = new stack_elt(eltType, v, top);
}

def stack.pop() : eltType {
  var v = top.value;
  top = top.next;
  return v;
}

def stack.writeThis(f : Writer) {
  var tmp = top;
  while tmp != nil {
    f.write(tmp.value, " ");
    tmp = tmp.next;
  }
}

var s : stack of string;
s.push("hello");
s.push("world");
writeln(s);
writeln(s.pop());
writeln(s);
