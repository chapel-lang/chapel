class stack_elt {
  type eltType;
  var value : eltType;
  var next : unmanaged stack_elt(eltType)?;
}

record stack {
  type eltType;

  var  top : unmanaged stack_elt(eltType)?;

  proc deinit() {
    while top != nil {
      var t = top;

      top = top!.next;

      delete t;
    }
  }
}

proc stack.empty
  return top == nil;

proc stack.push(v : eltType) {
  top = new unmanaged stack_elt(eltType, v, top);
}

proc stack.pop() : eltType {
  var t = top;
  var v = top!.value;

  top = top!.next;

  delete t;

  return v;
}

proc stack.writeThis(f) throws {
  var tmp = top;
  while tmp != nil {
    f.write(tmp!.value, " ");
    tmp = tmp!.next;
  }
}

var s : stack(string);

s.push("hello");
s.push("world");

writeln(s);
writeln(s.pop());
writeln(s);
