proc bad(type t, x:c_string) {
  try! {
    return string.createWithNewBuffer(x):t;
  }
}

bad(string, c"1"); // OK
