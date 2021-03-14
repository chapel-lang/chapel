class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f1 = new unmanaged foo("howdy");

f1.print();

var f2 = new unmanaged foo(3.2);

f2.print();


delete f2;
delete f1;
