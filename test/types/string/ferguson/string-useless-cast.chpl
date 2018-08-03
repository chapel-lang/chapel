proc bad(type t, x:c_string) {
  try! {
    //return _cast(t, x:string); // fails
    return x:string:t; // fails
    //var tmp = x:string; return tmp:string; // OK
  }
}

//bad(int, c"1"); // OK
bad(string, c"1"); // OK
