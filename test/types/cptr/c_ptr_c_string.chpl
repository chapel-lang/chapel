
use CTypes;

proc printit(arg:c_string) {
  extern proc printf(fmt: c_string, str: c_string);

  printf("%s\n", arg);
}

proc main() {
  var x:string = "hello";

  var cstr = x.c_str();
  var cstr_p : c_ptr(c_string) = c_ptrTo(cstr);
  var p1 = cstr:c_void_ptr;
  var p2 = cstr:c_ptr(c_char);
  var p3 = cstr:c_ptr(int(8));
  var p4 = cstr:c_ptr(uint(8));
  var p5 = cstr:c_ptrConst(c_char);
  var p6 = cstr:c_ptrConst(int(8));
  var p7 = cstr:c_ptrConst(uint(8));

  var p8 = cstr_p:c_ptr(c_ptr(uint(8)));
  var p9 = cstr_p:c_ptr(c_void_ptr);
  var p10 = cstr_p:c_ptrConst(c_ptr(uint(8)));

  var c1 = p1:c_string;
  var c2 = p2:c_string;
  var c3 = p3:c_string;
  var c4 = p4:c_string;
  var c5 = p5:c_string;
  var c6 = p6:c_string;
  var c7 = p7:c_string;

  var c8 = p8:c_ptr(c_string);
  var c9 = p9:c_ptr(c_string);
  var c10 = p10:c_ptrConst(c_string);

  printit(c1);
  printit(c2);
  printit(c3);
  printit(c4);
  printit(c5);
  printit(c6);
  printit(c7);

  printit(c8.deref());
  printit(c9.deref());
  printit(c10.deref());
}
