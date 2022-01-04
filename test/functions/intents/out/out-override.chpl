class C {
  var x: int;

  proc returnCopyOfMe() {
    return new unmanaged C(x);
  }
 
  proc returnCopyOfMeOutC(out arg) {
     arg = new unmanaged C(x);
  }

  proc returnCopyOfMeOut(out arg) {
     arg = new unmanaged C(x);
  }

}

class D: C {
  var y: int;

  override proc returnCopyOfMe() {
    return new unmanaged D(x,y);
  }
  override proc returnCopyOfMeOutC(out arg) {
    arg = (new unmanaged D(x,y)):C;
  }
  override proc returnCopyOfMeOut(out arg) {
    arg = new unmanaged D(x,y);
  }
}

proc main() {
  var a = new C(1);
  var b = new D(2,3);
  var c: owned C = new D(4,5);

  {
    var x = a.returnCopyOfMe();
    writeln(x, " : ", x.type:string);
    delete x;
  }
  {
    var y = b.returnCopyOfMe();
    writeln(y, " : ", y.type:string);
    delete y;
  }
  {
    var z = c.returnCopyOfMe();
    writeln(z, " : ", z.type:string);
    delete z;
  }

  {
    var x;
    a.returnCopyOfMeOutC(x);
    writeln(x, " : ", x.type:string);
    delete x;
  }
  {
    var y;
    b.returnCopyOfMeOutC(y);
    writeln(y, " : ", y.type:string);
    delete y;
  }
  {
    var z;
    c.returnCopyOfMeOutC(z);
    writeln(z, " : ", z.type:string);
    delete z;
  }

  {
    var x;
    a.returnCopyOfMeOut(x);
    writeln(x, " : ", x.type:string);
    delete x;
  }
  {
    var y;
    b.returnCopyOfMeOut(y);
    writeln(y, " : ", y.type:string);
    delete y;
  }
  {
    var z;
    c.returnCopyOfMeOut(z);
    writeln(z, " : ", z.type:string);
    delete z;
  }
}
