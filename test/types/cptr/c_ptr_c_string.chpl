use CPtr;
use SysCTypes;

proc printit(arg:c_string) {
  extern proc printf(fmt: c_string, str: c_string);

  printf("%s\n", arg);
}

proc main() {
  var x:string = "hello";

  var cstr = x.c_str();
  var p1 = cstr:c_void_ptr;
  var p2 = cstr:c_ptr(c_char);
  var p3 = cstr:c_ptr(int(8));
  var p4 = cstr:c_ptr(uint(8));

  var c1 = p1:c_string;
  var c2 = p2:c_string;
  var c3 = p3:c_string;
  var c4 = p4:c_string;

  printit(c1);
  printit(c2);
  printit(c3);
  printit(c4);
}
