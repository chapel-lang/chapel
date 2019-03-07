class ManyFields {
  var f2 : int;
  var f3 : int;
  var f4        = 12;
  var f5        =  3;

  proc init(val : int) {
    f2 = f5 + 4; // Depends on a later field's explicit initialization.  Uh oh!
    f3 = f4 - 2; // Depends on an omitted later field.  Uh oh!

    f5 = val;

  }
}

proc main() {
  var c: owned ManyFields = new owned ManyFields(2);

  writeln(c);
}
