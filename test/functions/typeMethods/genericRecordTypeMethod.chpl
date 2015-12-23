record MyRecord {
  type myType;
  type myTuple = (myType, myType);

  proc type firstElement(x:myTuple) return x(1);
}

var tup = (10,20);

writeln(MyRecord(int).firstElement(tup));

