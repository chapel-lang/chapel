class C {
  const n;

  //error: internal failure SYM1167 chpl Version 0.5.271
  var arr : [1..n] int = initArr(n);

  proc initArr(n) {
    const a : [1..n] int = [i in 1..n] i;
    return a;
  }
}

var c = new C(4);
writeln(c);
delete c;

