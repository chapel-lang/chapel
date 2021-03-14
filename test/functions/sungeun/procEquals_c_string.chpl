proc =(ref a: c_string, b:string) {
  __primitive("=", a, b.c_str());
}
