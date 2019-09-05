module test {

  record MyRecord { var x: int; }

  class MyClass { var x:int; }

  proc main() {

    var own = new owned MyClass(1);
    var ownQ:owned MyClass? = new owned MyClass(1);
    var borrow = own.borrow();
    var borrowQ = ownQ.borrow();

    var shr = new shared MyClass(1);
    var shrQ:shared MyClass? = shr;

    var unm = new unmanaged MyClass(1);
    var unmQ:unmanaged MyClass? = unm;

    var r = new MyRecord(1);

    var one = 1;

    writeln("own:", own.type:string);
    writeln("borrow:", borrow.type:string);
    writeln("shr:", shr.type:string);
    writeln("unm:", unm.type:string);
    writeln("ownQ:", ownQ.type:string);
    writeln("borrowQ:", borrowQ.type:string);
    writeln("shrQ:", shrQ.type:string);
    writeln("unmQ:", unmQ.type:string);
    writeln("r:", r.type:string);
    writeln("one:", one.type:string);

    writeln();
    writeln();
    writeln("=== isNilableClass ===");
    writeln("own ", isNilableClass(own), " ",
                    isNilableClass(own.type));
    writeln("borrow ", isNilableClass(borrow), " ",
                       isNilableClass(borrow.type));
    writeln("shr ", isNilableClass(shr), " ",
                    isNilableClass(shr.type));
    writeln("unm ", isNilableClass(unm), " ",
                    isNilableClass(unm.type));
    writeln();
    writeln("ownQ ", isNilableClass(ownQ), " ",
                     isNilableClass(ownQ.type));
    writeln("borrowQ ", isNilableClass(borrowQ), " ",
                        isNilableClass(borrowQ.type));
    writeln("shrQ ", isNilableClass(shrQ), " ",
                     isNilableClass(shrQ.type));
    writeln("unmQ ", isNilableClass(unmQ), " ",
                     isNilableClass(unmQ.type));
    writeln();
    writeln("r ", isNilableClass(r), " ",
                   isNilableClass(r.type));
    writeln("one ", isNilableClass(one), " ",
                    isNilableClass(one.type));
    writeln();
    writeln("owned MyClass ", isNilableClass(owned MyClass));
    writeln("borrowed MyClass ", isNilableClass(borrowed MyClass));
    writeln("shared MyClass ", isNilableClass(shared MyClass));
    writeln("unmanaged MyClass ", isNilableClass(unmanaged MyClass));
    writeln();
    writeln("owned MyClass? ", isNilableClass(owned MyClass?));
    writeln("borrowed MyClass? ", isNilableClass(borrowed MyClass?));
    writeln("shared MyClass? ", isNilableClass(shared MyClass?));
    writeln("unmanaged MyClass? ", isNilableClass(unmanaged MyClass?));
    writeln();
    writeln("MyRecord ", isNilableClass(MyRecord));
    writeln("int ", isNilableClass(int));
    writeln();
    writeln();
    writeln("=== isNonNilableClass ===");
    writeln("own ", isNonNilableClass(own), " ",
                    isNonNilableClass(own.type));
    writeln("borrow ", isNonNilableClass(borrow), " ",
                    isNonNilableClass(borrow.type));
    writeln("shr ", isNonNilableClass(shr), " ",
                    isNonNilableClass(shr.type));
    writeln("unm ", isNonNilableClass(unm), " ",
                    isNonNilableClass(unm.type));
    writeln();
    writeln("ownQ ", isNonNilableClass(ownQ), " ",
                    isNonNilableClass(ownQ.type));
    writeln("borrowQ ", isNonNilableClass(borrowQ), " ",
                    isNonNilableClass(borrowQ.type));
    writeln("shrQ ", isNonNilableClass(shrQ), " ",
                    isNonNilableClass(shrQ.type));
    writeln("unmQ ", isNonNilableClass(unmQ), " ",
                    isNonNilableClass(unmQ.type));
    writeln();
    writeln("r ", isNonNilableClass(r), " ",
                    isNonNilableClass(r.type));
    writeln("one ", isNonNilableClass(one), " ",
                    isNonNilableClass(one.type));
    writeln();
    writeln("owned MyClass ", isNonNilableClass(owned MyClass));
    writeln("borrowed MyClass ", isNonNilableClass(borrowed MyClass));
    writeln("shared MyClass ", isNonNilableClass(shared MyClass));
    writeln("unmanaged MyClass ", isNonNilableClass(unmanaged MyClass));
    writeln();
    writeln("owned MyClass? ", isNonNilableClass(owned MyClass?));
    writeln("borrowed MyClass? ", isNonNilableClass(borrowed MyClass?));
    writeln("shared MyClass? ", isNonNilableClass(shared MyClass?));
    writeln("unmanaged MyClass? ", isNonNilableClass(unmanaged MyClass?));
    writeln();
    writeln("MyRecord ", isNonNilableClass(MyRecord));
    writeln("int ", isNonNilableClass(int));

    delete unm;
  }
}
