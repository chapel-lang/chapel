record R {
  type foo;

}

type t = R(bool);
type typeField = __primitive("query type field", t, "foo");
writeln(typeField:string);
