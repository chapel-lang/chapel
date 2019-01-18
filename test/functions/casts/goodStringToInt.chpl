config const x: string;
var i: int;
try {
  i = x: int;
} catch {
  writeln("fail: caught casting error");
}
