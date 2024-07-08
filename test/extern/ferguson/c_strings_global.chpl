use CTypes;

extern proc returns_c_string():c_ptrConst(c_char);
extern proc returns_c_string_in_argument(ref ret:c_ptrConst(c_char));
extern proc print_c_string(arg:c_ptrConst(c_char));
extern proc print_c_string_len(arg:c_ptrConst(c_char), len:c_int);
extern proc return_c_string_in_argument_with_length(ref ret:c_ptrConst(c_char), ref len:c_int);

var str = "Hello";

proc go() {
  writeln("Should be Hello:");
  print_c_string(str.c_str());

  writeln("Should be returned_c_string x3");
  var gotc = returns_c_string();
  print_c_string(gotc);

  writeln(string.createBorrowingBuffer(gotc));

  var gots = string.createBorrowingBuffer(gotc);
  writeln(gots);

  writeln("Should be returned_c_string_in_argument x3");
  var argc:c_ptrConst(c_char);
  returns_c_string_in_argument(argc);

  print_c_string(argc);

  writeln(string.createBorrowingBuffer(argc));

  var args = string.createBorrowingBuffer(argc);
  writeln(args);

  writeln("Should be returned_c_string_in_argument_with_length x3");
  var arg2c:c_ptrConst(c_char);
  var len:c_int;
  return_c_string_in_argument_with_length(arg2c, len);

  print_c_string_len(arg2c, len);

  writeln((string.createBorrowingBuffer(arg2c))[0..len-1]);

  var arg2s = (string.createBorrowingBuffer(arg2c))[0..len-1];
  writeln(arg2s);
}

go();
