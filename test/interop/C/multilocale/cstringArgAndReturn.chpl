// Chapel file that exports functions: one that takes an c_string, one that
// returns an c_string, and one that does both.
export proc take(in x: c_string) {
  writeln(x: string);
}

export proc give(): c_string {
  var ret: c_string = "I return unto you a string";
  return ret;
}

export proc takeAndReturn(in x: c_string): c_string {
  return x;
}
