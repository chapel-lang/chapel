class stack_elt {
  type elt_type;
  var value : elt_type;
  var next : stack_elt(elt_type);
}

record stack {
  type elt_type;
  var top : stack_elt(elt_type);
}

def stack.empty
  return top == nil;

def stack.push(v : elt_type) {
  top = stack_elt(elt_type, v, top);
}

def stack.pop() : elt_type {
  var v = top.value;
  top = top.next;
  return v;
}

def stack.write(f : file) {
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
