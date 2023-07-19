proc f():c_string {
  return "Hello";
}

var x = f();
writeln(string.createCopyingBuffer(x), " ", x.type:string);

use CTypes;
proc f_ptr():c_ptrConst(c_char) {
  return "Hello";
}

var x_ptr = f_ptr();
writeln(string.createCopyingBuffer(x_ptr), " ", x_ptr.type:string);
