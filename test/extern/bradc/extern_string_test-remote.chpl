use IO, CTypes;

extern proc return_string_test():chpl_c_string;
extern proc return_string_arg_test(ref arg: chpl_c_string);

writeln("returned string ",string.createCopyingBuffer(return_string_test():c_ptrConst(c_char)));
stdout.flush();

var s:string;
on Locales(1) {
  var temp_cs: chpl_c_string;
  return_string_arg_test(temp_cs);
  var temp_s:string = string.createCopyingBuffer(temp_cs:c_ptrConst(c_char));
  s = temp_s;
}
writeln("returned string arg ",s);

{
  use CTypes;
  extern proc return_c_ptrConstUchar_test():c_ptrConst(c_char);
  extern proc return_c_ptrConstUchar_arg_test(ref str: c_ptrConst(c_char));

  writeln("returned string ",string.createCopyingBuffer(return_c_ptrConstUchar_test()));
  stdout.flush();

  var s:string;
  on Locales(1) {
    var temp_cs: c_ptrConst(c_char);
    return_c_ptrConstUchar_arg_test(temp_cs);
    var temp_s:string = string.createCopyingBuffer(temp_cs);
    s = temp_s;
  }
  writeln("returned string arg ",s);
}

{
  use CTypes;
  extern proc return_c_ptrConstChar_test():c_ptrConst(c_char);
  extern proc return_c_ptrConstChar_arg_test(ref str: c_ptrConst(c_char));

  writeln("returned string ",string.createCopyingBuffer(return_c_ptrConstChar_test()));
  stdout.flush();

  var s:string;
  on Locales(1) {
    var temp_cs: c_ptrConst(c_char);
    return_c_ptrConstChar_arg_test(temp_cs);
    var temp_s:string = string.createCopyingBuffer(temp_cs);
    s = temp_s;
  }
  writeln("returned string arg ",s);
}

{
  use CTypes;
  extern proc return_c_ptrUchar_test():c_ptr(c_uchar);
  extern proc return_c_ptrUchar_arg_test(ref str: c_ptr(c_uchar));

  writeln("returned string ",string.createCopyingBuffer(return_c_ptrUchar_test()));
  stdout.flush();

  var s:string;
  on Locales(1) {
    var temp_cs: c_ptr(c_uchar);
    return_c_ptrUchar_arg_test(temp_cs);
    var temp_s:string = string.createCopyingBuffer(temp_cs);
    s = temp_s;
  }
  writeln("returned string arg ",s);
}

{
  use CTypes;
  extern proc return_c_ptrChar_test():c_ptr(c_char);
  extern proc return_c_ptrChar_arg_test(ref str: c_ptr(c_char));

  writeln("returned string ",string.createCopyingBuffer(return_c_ptrChar_test()));
  stdout.flush();

  var s:string;
  on Locales(1) {
    var temp_cs: c_ptr(c_char);
    return_c_ptrChar_arg_test(temp_cs);
    var temp_s:string = string.createCopyingBuffer(temp_cs);
    s = temp_s;
  }
  writeln("returned string arg ",s);
}
