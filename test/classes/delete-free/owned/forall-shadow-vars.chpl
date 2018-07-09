class MyClass {
  var x:int;
}

config const n = 4;
proc test() {

  {
    var outerOwnedMyClass = new owned MyClass(1);
    var outerSharedMyClass = new shared MyClass(2);

    writeln("forall blank owned");
    forall i in 1..n {
      writeln(outerOwnedMyClass.type:string);
    }

    writeln("forall blank shared");
    forall i in 1..4 {
      writeln(outerSharedMyClass.type:string);
    }

    writeln("forall const owned");
    forall i in 1..4 with (const outerOwnedMyClass) {
      writeln(outerOwnedMyClass.type:string);
    }

    writeln("forall const ref owned");
    forall i in 1..4 with (const ref outerOwnedMyClass) {
      writeln(outerOwnedMyClass.type:string);
    }

    writeln("forall ref owned");
    forall i in 1..4 with (ref outerOwnedMyClass) {
      writeln(outerOwnedMyClass.type:string);
    }
  }
}

test();
