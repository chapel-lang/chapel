class stack_elt {
  type elt_type;
  var value : elt_type;
  var next : stack_elt(elt_type);
}

record stack {
  type elt_type;
  var top : stack_elt(elt_type);
}

function stack.empty
  return top == nil;

function stack.push(v : elt_type) {
  top = stack_elt(elt_type, v, top);
}

function stack.pop() : elt_type {
  var v = top.value;
  top = top.next;
  return v;
}

function fwrite(f : file, s : stack) {
  var tmp = s.top;
  while tmp != nil {
    write(tmp.value, " ");
    tmp = tmp.next;
  }
}

var s : stack of string;
s.push("hello");
s.push("world");
writeln(s);
writeln(s.pop());
writeln(s);
