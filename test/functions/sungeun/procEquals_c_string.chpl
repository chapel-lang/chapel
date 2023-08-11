use CTypes;
operator =(ref a: c_ptrConst(c_char), b:string) {
  __primitive("=", a, b.c_str());
}
