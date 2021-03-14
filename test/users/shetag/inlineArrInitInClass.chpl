class A {
    const k;
    const arrD = {0..k-1};
    const arrA: [arrD] int = [i in arrD]i*2;
}

proc main() {
  const a = new unmanaged A(6);

  writeln(a);

  delete a;
}
