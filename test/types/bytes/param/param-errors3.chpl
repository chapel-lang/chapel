//cannot coerce bytes to string
param b = b"some bytes";
proc foo(param s: string) {
  writeln(s, " as ", s.type:string);
}
foo(b);

