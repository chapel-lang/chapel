record R {
  type field;
}

var r = new R((int, real));
writeln(r.field[0]:string);
writeln(r.field[1]:string);
