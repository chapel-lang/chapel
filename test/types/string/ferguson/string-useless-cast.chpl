proc bad(type t, x:c_string) {
  try! {
    return createStringWithNewBuffer(x):t;
  }
}

bad(string, c"1"); // OK
