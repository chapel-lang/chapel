use Map;
use Set;

//
// NOTE: this test does not print anything by default. there is not way to
//       guarantee the order of the output, or even if elements are
//       consistently paired.
//

config const n = 100;
config const printValues = false;

var m1 = new map(int, int);
for i in 1..n {
  m1[i] = i;
}
var m2 = new map(string, string);
for i in 1..n {
  m2[i:string] = i:string;
}

var s = new set(int);
for i in 1..n {
  s.add(i);
}


writeln("Can I zip .keys with a set");
for (key, val) in zip(m1.keys(), s) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");
forall (key, val) in zip(m1.keys(), s) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");


writeln("Can I zip .values with a set");
for (key, val) in zip(m1.values(), s) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");
forall (key, val) in zip(m1.values(), s) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");


writeln("Can I zip .keys with .values");
for (key, val) in zip(m1.keys(), m1.values()) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");
forall (key, val) in zip(m1.keys(), m1.values()) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");


writeln("Can I zip .values with .keys");
for (val, key) in zip(m1.values(), m1.keys()) {
  if printValues then writeln("  ", val, " ", key);
}
writeln("===========");
forall (val, key) in zip(m1.values(), m1.keys()) {
  if printValues then writeln("  ", val, " ", key);
}
writeln("===========");


writeln("Can I zip .keys with another .keys");
for (key1, key2) in zip(m1.keys(), m2.keys()) {
  if printValues then writeln("  ", key1, " ", key2);
}
writeln("===========");
forall (key1, key2) in zip(m1.keys(), m2.keys()) {
  if printValues then writeln("  ", key1, " ", key2);
}
writeln("===========");

writeln("Can I zip .values with another .values");
for (val1, val2) in zip(m1.values(), m2.values()) {
  if printValues then writeln("  ", val1, " ", val2);
}
writeln("===========");
forall (val1, val2) in zip(m1.values(), m2.values()) {
  if printValues then writeln("  ", val1, " ", val2);
}
writeln("===========");


writeln("Can I zip .keys with another .values");
for (key, val) in zip(m1.keys(), m2.values()) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");
forall (key, val) in zip(m1.keys(), m2.values()) {
  if printValues then writeln("  ", key, " ", val);
}
writeln("===========");


writeln("Can I zip .values with another .keys");
for (val, key) in zip(m1.values(), m2.keys()) {
  if printValues then writeln("  ", val, " ", key);
}
writeln("===========");
forall (val, key) in zip(m1.values(), m2.keys()) {
  if printValues then writeln("  ", val, " ", key);
}
writeln("===========");
