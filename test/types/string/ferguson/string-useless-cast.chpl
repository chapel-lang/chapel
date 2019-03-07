proc bad(type t, x:c_string) {
  try! {
    return x:string:t;
  }
}

bad(string, c"1"); // OK
