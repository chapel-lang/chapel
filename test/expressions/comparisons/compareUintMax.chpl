// regression test for user issue using old param compare logic
config var foo : uint(64) = max(uint(64));
//Logic to compute foo replaced with config
writeln("foo: ", foo, " vs ", max(uint(64)));
writeln("types: ", foo.type : string, " vs ", max(uint(64)).type :string);
if (foo < max(uint(64))) {
  writeln("passed if");
} else {
  writeln("shouldn't reach");
}
