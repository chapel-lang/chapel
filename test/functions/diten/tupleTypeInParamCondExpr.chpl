param useInt = false;

proc getType(type t) type {
  return if useInt then int else t;
}

var a: getType(2*int);
writeln(a.type:string);
