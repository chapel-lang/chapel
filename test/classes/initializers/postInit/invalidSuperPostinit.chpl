
class A {
  var x : int;

  proc postinit() {
    writeln("A.postinit");
  }
}

class B : A {
  proc postinit() {
    if false then super.postinit();
  }
}

class C : A {
  proc postinit() {
    if false then super.postinit();
    else writeln();
  }
}

class D : A {
  proc postinit() {
    super.postinit();
    super.postinit();
  }
}

class E : A {
  proc postinit() {
    for i in 1..10 do
      super.postinit();

    var i = 0;
    do {
      super.postinit();
      i += 1;
    } while i < 10;

    i = 0;
    while i < 10 {
      super.postinit();
      i += 1;
    }

    forall i in 1..10 do
      super.postinit();

    for param i in 1..10 do
      super.postinit();
  }
}

class F : A {
  proc postinit() {
    coforall i in 1..10 do
      super.postinit();
    coforall i in 1..10 do on here do
      super.postinit();

    begin super.postinit();
    begin on here do super.postinit();

    cobegin {
      super.postinit();
      writeln();
    }
  }
}
