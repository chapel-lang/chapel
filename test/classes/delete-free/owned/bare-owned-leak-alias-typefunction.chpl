class C {}

type t = owned C;

new t();

proc getType() type {
  return owned C;
}

new getType()();

