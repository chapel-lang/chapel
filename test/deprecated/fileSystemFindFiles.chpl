use FileSystem;

proc foo(f: string) { }

for f in findfiles() {
  foo(f);
}

forall f in findfiles() {
  foo(f);
}
