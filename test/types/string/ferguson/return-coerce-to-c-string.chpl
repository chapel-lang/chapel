proc f():c_string {
  return "Hello";
}

var x = f();
writeln(createStringWithNewBuffer(x), " ", x.type:string);
