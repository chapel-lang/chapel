// GitHub issue #19674: ensure that enumerateStatesFixedHamming_v2,v3()
// are invoked only once per source-program call.

var global_count : atomic int = 0;

proc enumerateStatesFixedHamming_v2() {
  writeln("Calling enumerateStatesFixedHamming ...");
  global_count.add(1);
  var rs : [0 ..# 10] uint(64);
  return rs;
}

proc localEnumerateRepresentatives_v2() : [] uint(64) {
  writeln("inside localEnumerateRepresentatives ...");
  return enumerateStatesFixedHamming_v2();
}

// a variant on the above with an extra art
proc enumerateStatesFixedHamming_v3(arg:int) {
  writeln("Calling enumerateStatesFixedHamming v3 ...");
  global_count.add(1);
  var rs : [0 ..# 10] uint(64);
  return rs;
}

proc localEnumerateRepresentatives_v3() : [] uint(64) {
  writeln("inside localEnumerateRepresentatives v3 ...");
  return enumerateStatesFixedHamming_v3(anotherfun());
}

proc anotherfun() do return 234;

proc specifyDomain_v2(): [0..#10] {
  writeln("inside specifyDomain_v2");
  return enumerateStatesFixedHamming_v2();
}

proc specifyDomain_v3(): [0..#10] {
  writeln("inside specifyDomain_v3");
  return enumerateStatesFixedHamming_v3(anotherfun());
}

// specifyDomainAndEltype_* do not exercise the code paths in the compiler
// that caused the bug in #19674. This is because the only way to specify
// both the domain and element type in the return type (that I know of)
// is to make the return type concrete, in which case the compiler inserts
// a coercion from the return value to the return type.
// We check this case as well, for completeness.

proc specifyDomainAndEltype_v2(): [0..#10] uint(64) {
  writeln("inside specifyDomainAndEltype_v2");
  return enumerateStatesFixedHamming_v2();
}

proc specifyDomainAndEltype_v3(): [0..#10] uint(64) {
  writeln("inside specifyDomainAndEltype_v3");
  return enumerateStatesFixedHamming_v3(anotherfun());
}

proc main() {
  writeln(global_count);
  const r = localEnumerateRepresentatives_v2();
  const q = localEnumerateRepresentatives_v3();
  const a = specifyDomain_v2();
  const b = specifyDomain_v3();
  const c = specifyDomainAndEltype_v2();
  const d = specifyDomainAndEltype_v3();
  writeln(global_count);
}
