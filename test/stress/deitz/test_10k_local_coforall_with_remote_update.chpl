config const n = 10000;

class C {
  var A: [1..n] int;
}

var c: unmanaged C?;

on Locales(1) {
  c = new unmanaged C();
}

coforall i in 1..n {
  c!.A(i) += 1;
}

for i in 1..n do
  if c!.A(i) != 1 then
    halt("error detected");

writeln("success");

delete c;
