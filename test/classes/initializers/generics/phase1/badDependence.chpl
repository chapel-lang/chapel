class ManyFields {
  param f2 : int;
  param f3 : int;
  param f4        = 12;
  param f5        =  3;

  proc init(param val : int) {
    f2 = f5 + 4; // Depends on a later field's explicit initialization.  Uh oh!
    f3 = f4 - 2; // Depends on an omitted later field.  Uh oh!

    f5 = val;

  }
}

proc main() {
  var c = new borrowed ManyFields(2);

  writeln(c.type: string);
}
