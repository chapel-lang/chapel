class MyClass {
  var x:int;
}

config const n = 4;
proc test() {

  {
    var outerOwnedMyClass = new owned MyClass(1);
    var outerSharedMyClass = new shared MyClass(2);

    writeln("blank owned");
    writeln(" coforall");
    coforall i in 1..n {
      writeln("  ", outerOwnedMyClass.type:string);
    }
    writeln(" cobegin");
    cobegin {
      writeln("  ", outerOwnedMyClass.type:string);
      writeln("  ", outerOwnedMyClass.type:string);
    }
    writeln("blank shared");
    writeln(" coforall");
    coforall i in 1..4 {
      writeln("  ", outerSharedMyClass.type:string);
    }
    writeln(" cobegin");
    cobegin {
      writeln("  ", outerSharedMyClass.type:string);
      writeln("  ", outerSharedMyClass.type:string);
    }

    writeln("const ref owned");
    writeln(" coforall");
    coforall i in 1..4 with (const ref outerOwnedMyClass) {
      writeln("  ", outerOwnedMyClass.type:string);
    }

    writeln("ref owned");
    writeln(" coforall");
    coforall i in 1..4 with (ref outerOwnedMyClass) {
      writeln("  ", outerOwnedMyClass.type:string);
    }


    writeln("blank begin");
    sync {
      begin {
        writeln("  ", outerOwnedMyClass.type:string);
        writeln("  ", outerSharedMyClass.type:string);
      }
    }
    
    writeln("const ref begin");
    sync {
      begin with (const ref outerOwnedMyClass, const ref outerSharedMyClass) {
        writeln("  ", outerOwnedMyClass.type:string);
        writeln("  ", outerSharedMyClass.type:string);
      }
    }
    
    writeln("ref begin");
    sync {
      begin with (ref outerOwnedMyClass, ref outerSharedMyClass) {
        writeln("  ", outerOwnedMyClass.type:string);
        writeln("  ", outerSharedMyClass.type:string);
      }
    }
  }

  {
    var outerOwnedMyClass: owned MyClass? = new owned MyClass(1);
    var outerSharedMyClass = new shared MyClass(2);

    writeln("in begin");
    sync {
      begin with (in outerOwnedMyClass, in outerSharedMyClass) {
        writeln("  ", outerOwnedMyClass.type:string);
        writeln("  ", outerSharedMyClass.type:string);
      }
    }
    writeln(outerOwnedMyClass);
  }
  
  {
    var outerOwnedMyClass: owned MyClass? = new owned MyClass(1);
    var outerSharedMyClass = new shared MyClass(2);

    writeln("const in begin");
    sync {
      begin with (const in outerOwnedMyClass, const in outerSharedMyClass) {
        writeln("  ", outerOwnedMyClass.type:string);
        writeln("  ", outerSharedMyClass.type:string);
      }
    }
    writeln(outerOwnedMyClass);
  }
}

test();
