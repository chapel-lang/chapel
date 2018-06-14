var A:[1..10] int;

proc tf(type t) type {
  return t.type;
}

tf(A.type);
