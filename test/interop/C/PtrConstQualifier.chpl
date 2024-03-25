use CTypes;

export proc printAndReturn(x: c_ptrConst(int(32))) {
  writeln(x.deref():string);
  return x;
}
