module OuterModule {
  module M1 {
    class MyType {
      var x:int;
    }
  }

  module M2 {
    class MyType {
      var y:int;
    }
  }

  use M1;
  use M2;

  var m1 = new owned M1.MyType(x=1);
  var m2 = new owned M2.MyType(y=2);

  writeln(m1.x);
  writeln(m2.y);
}
