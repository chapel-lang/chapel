
record DR {
  param cond : bool;
  var x : if cond then int else real;
  var y : if cond then int else nothing;
}

record ER {
  param cond : bool;
  var x : if cond then int else real;
  var y : if cond then int else nothing;

  proc init(param cond : bool) {
    this.cond = cond;
  }
}

{
  var t = new DR(true);
  writeln("t = ", t);

  var f = new DR(false);
  writeln("f = ", f);
}
{
  var t = new ER(true);
  writeln("t = ", t);

  var f = new ER(false);
  writeln("f = ", f);
}
