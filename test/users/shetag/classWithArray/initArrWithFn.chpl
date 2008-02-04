class C {
  const n;

  //error: internal failure SYM1167 chpl Version 0.5.271
  var arr : [1..n] int = initArr(n);
  //  var arr : [1..n] int;

  def initialize() {
    //    [i in 1..n] arr(i) = i;
  }

  def initArr(n) {
    const a : [i in 1..n] int = i;
    return a;
  }
}

var c = new C(4);
writeln(c);

