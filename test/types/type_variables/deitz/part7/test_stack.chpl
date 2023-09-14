class stack_elt {
  type eltType;
  var value : eltType;
  var next : unmanaged stack_elt(eltType)?;
}

record stack : writeSerializable {
  type eltType;

  var  top : unmanaged stack_elt(eltType)?;

  proc ref deinit() {
    while top != nil {
      var t = top;

      top = top!.next;

      delete t;
    }
  }
}

proc stack.empty do
  return top == nil;

proc ref stack.push(v : eltType) {
  top = new unmanaged stack_elt(eltType, v, top);
}

proc ref stack.pop() : eltType {
  var t = top;
  var v = top!.value;

  top = top!.next;

  delete t;

  return v;
}

proc stack.serialize(writer, ref serializer) throws {
  var tmp = top;
  while tmp != nil {
    writer.write(tmp!.value, " ");
    tmp = tmp!.next;
  }
}

var s : stack(string);

s.push("hello");
s.push("world");

writeln(s);
writeln(s.pop());
writeln(s);
