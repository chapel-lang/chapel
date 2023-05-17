private use ChapelSysCTypes, CTypes;

operator =(ref a: c_string, b:string) {
  __primitive("=", a, b:c_ptrConst(c_char):c_string);
}
