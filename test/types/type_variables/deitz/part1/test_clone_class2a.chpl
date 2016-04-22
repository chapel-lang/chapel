class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f1 = new foo("howdy");

f1.print();

var f2 = new foo(3.2);

f2.print();


delete f2;
delete f1;
