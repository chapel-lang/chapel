use CTypes;
proc bad(type t, x:c_ptrConst(c_char)) {
  try! {
    return string.createCopyingBuffer(x):t;
  }
}

bad(string, c"1"); // OK

bad(string, "1"); // OK
