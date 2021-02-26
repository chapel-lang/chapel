{
  class MyClass {
    var D;
    var A: [D] int;

    /*
    // uncomment this to get the desired behavior
    proc init(d) {
      this.D = d;
    }
    */
  }

  var arr = [1,2,3];
  var c = new MyClass(arr.domain);
  c.D = {1..5};

  writeln("These should be different:");
  writeln(c.A.domain);
  writeln(arr.domain);
}

{
  record MyRecord {
    var D;
    var A: [D] int;

    /*
    // uncomment this to get the desired behavior
    proc init(d) {
      this.D = d;
    }
    */
  }

  var arr = [1,2,3];
  var c = new MyRecord(arr.domain);
  c.D = {1..5};

  writeln("These should be different:");
  writeln(c.A.domain);
  writeln(arr.domain);
}
