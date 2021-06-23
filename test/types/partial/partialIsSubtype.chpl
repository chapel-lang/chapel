class QuiteGeneric {
  type t;
  param p: int;
  var x;
}

type partial1 = QuiteGeneric(int);
writeln(partial1: string);
writeln(isSubtype(partial1, QuiteGeneric)); // true

type partial2 = QuiteGeneric(int, 3);
writeln(partial2: string);
writeln(isSubtype(partial2, QuiteGeneric)); // true
writeln(isSubtype(partial2, partial1)); // true
writeln(isSubtype(partial1, partial2)); // false

type complete = QuiteGeneric(int, 3, real);
writeln(complete: string);
writeln(isSubtype(complete, QuiteGeneric)); // true
writeln(isSubtype(complete, partial1)); // true
writeln(isSubtype(complete, partial2)); // true
writeln(isSubtype(partial1, complete)); // false
writeln(isSubtype(partial2, complete)); // false
