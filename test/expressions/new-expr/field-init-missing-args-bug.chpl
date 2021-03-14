class C { }

var A: [1..10] owned C = for 1..10 do new C();

class D {
  var A: [1..10] owned C = for 1..10 do new C();
}

class E {
  var A: [1..10] owned C;

  proc init() {
    this.A = for i in 1..10 do new C();
  }
}

class F {
  var A: [1..10] owned C = [1..10] new C();
}

class G {
  var A: [1..10] owned C = [1..10] new owned C();
}

class H {
  var A: [1..10] owned C = for 1..10 do new owned C();
}

var B: [1..10] owned C = for 1..10 do new owned C();

writeln(A);
writeln(B);

var myD = new D();
writeln(myD);

var myE = new E();
writeln(myE);

var myF = new F();
writeln(myF);

var myG = new G();
writeln(myG);

var myH = new H();
writeln(myH);
