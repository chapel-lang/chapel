proc f():c_string {
  return "Hello";
}

var x = f();
writeln(x:string, " ", x.type:string);
