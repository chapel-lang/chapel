operator =(ref a: c_string, b:string) {
  __primitive("=", a, c_ptrToConst_helper(b):c_string);
}
