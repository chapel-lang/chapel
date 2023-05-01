proc f():c_string {
  return "Hello";
}

var x = f();
writeln(string.createCopyingBuffer(x), " ", x.type:string);
