use CTypes;
proc bad(type t, x:c_ptrConst(c_char)) {
  try! {
    return string.createCopyingBuffer(x):t;
  }
}

bad(string, "1".c_str()); // OK

bad(string, "1"); // OK
