proc noReturnAtAll(z: string) {
  param EMPTY = '';
  var str = if z == '~' then ' ' else (if z == '+' then z else EMPTY);
  writeln(str);
}

noReturnAtAll("+");
