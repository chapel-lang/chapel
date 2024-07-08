proc noReturnAtAll(z: string) {
  param EMPTY = '';
  const ref str = if z == '~' then ' ' else (if z == '+' then z else EMPTY);
  writeln(str);
}

noReturnAtAll("+");
