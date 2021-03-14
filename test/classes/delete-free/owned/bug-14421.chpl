class MyInt {
  var x: int;
}

type MyManagedInt = owned MyInt;
type MyManagedIntMaybe = owned MyInt?;

writeln(new MyManagedInt(1));
writeln(new MyManagedIntMaybe(2));
writeln(new MyManagedInt?(3));
