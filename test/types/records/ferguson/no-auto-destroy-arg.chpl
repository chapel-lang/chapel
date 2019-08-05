record someRecord {
  var dummy = 0;

  proc deinit() {
    writeln("Destroying record!");
  }
}

proc someMethod(pragma "no auto destroy" in x: someRecord) {
  return;
}

var x = new someRecord();

for i in 1..8 do
  someMethod(x);
