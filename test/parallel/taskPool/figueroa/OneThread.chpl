config const n = 10;
var total: sync int = 0;

proc foo (x) {
  total.writeEF(total.readFE() + x);
}

coforall i in 1..n do
  cobegin {
    foo(i);
    foo(i+n);
  }

writeln ("total is ", total.readFF());
