var total: sync int = 0;

coforall i in 1..3 {
  var subtotal: sync int = 0;
  coforall i in 1..3 {
    writeln ("Hello, world!");
    subtotal.writeEF(subtotal.readFE() + i);
  }
  total.writeEF(total.readFE() + subtotal.readFE());
}

writeln ("Bye no. ", total.readFF(), "!");
