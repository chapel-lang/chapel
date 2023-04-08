proc f():c_string {
  return "Hello";
}

var x = f();
writeln(string.createWithNewBuffer(x), " ", x.type:string);
