var total: sync int = 0;

coforall i in 1..3 {
  var subtotal: sync int = 0;
  coforall i in 1..3 {
    writeln ("Hello, world!");
    subtotal += i;
  }
  total += subtotal;
}

writeln ("Bye no. ", total.readFF(), "!");
