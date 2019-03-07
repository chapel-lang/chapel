class Foo {
  const arrDom: domain(1);
  const arr: [arrDom] int;

  proc init(val: int) {
    arrDom = {1..val};
    this.complete();
    for i in arrDom do
      arr[i] = i;
  }
}

var foo = new unmanaged Foo(10);
writeln(foo);
delete foo;
