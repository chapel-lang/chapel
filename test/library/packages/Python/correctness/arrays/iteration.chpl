use Python, CTypes;

var interp = new Interpreter();

// use all 1's so we don't have to deal with different printing orders
var lst = interp.get('list')([1, 1, 1]);
var arr = interp.importModule('array')
                .get('array')(owned PyArray(c_int, 1), 'i', lst);

writeln("list:");
for i in lst {
  writeln(i);
}

writeln("array.values (default, for):");
for i in arr.values() {
  writeln(i);
}
writeln("array.values (default, foreach):");
foreach i in arr.values() {
  writeln(i);
}
writeln("array.values (default, forall):");
forall i in arr.values() {
  writeln(i);
}

writeln("array.values (eltType, for):");
for i in arr.values(c_int) {
  writeln(i);
}
writeln("array.values (eltType, foreach):");
foreach i in arr.values(c_int) {
  writeln(i);
}
writeln("array.values (eltType, forall):");
forall i in arr.values(c_int) {
  writeln(i);
}

// TODO: maybe in the future these will not work
writeln("array.these (default, for):");
for i in arr {
  writeln(i);
}
writeln("array.these (eltType, for):");
for i in arr.these(c_int) {
  writeln(i);
}
