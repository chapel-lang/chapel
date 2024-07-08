
use CTypes;

proc printit(arg:c_ptrConst(c_char)) {
  extern proc printf(fmt: c_ptrConst(c_char), str: c_ptrConst(c_char));

  printf("%s\n", arg);
}

proc main() {
  var x:string = "hello";

  var cstr = x.c_str();
  var cstr_p : c_ptr(c_ptrConst(c_char)) = c_ptrTo(cstr);
  var p1 = cstr:c_ptr(void);
  var p2 = cstr:c_ptr(c_char);
  var p3 = cstr:c_ptr(int(8));
  var p4 = cstr:c_ptr(uint(8));
  var p5 = cstr:c_ptrConst(c_char);
  var p6 = cstr:c_ptrConst(int(8));
  var p7 = cstr:c_ptrConst(uint(8));

  var p8 = cstr_p:c_ptr(c_ptr(uint(8)));
  var p9 = cstr_p:c_ptr(c_ptr(void));
  var p10 = cstr_p:c_ptrConst(c_ptr(uint(8)));

  var c1 = p1:c_ptrConst(c_char);
  var c2 = p2:c_ptrConst(c_char);
  var c3 = p3:c_ptrConst(c_char);
  var c4 = p4:c_ptrConst(c_char);
  var c5 = p5:c_ptrConst(c_char);
  var c6 = p6:c_ptrConst(c_char);
  var c7 = p7:c_ptrConst(c_char);

  var c8 = p8:c_ptr(c_ptrConst(c_char));
  var c9 = p9:c_ptr(c_ptrConst(c_char));
  var c10 = p10:c_ptrConst(c_ptrConst(c_char));

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
