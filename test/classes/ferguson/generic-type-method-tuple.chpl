record MyRecord {
  type myType;
  type myTuple = (myType, myType);

  proc type firstElement(x:myTuple) type return x(1).type;
}

var tup = (10,20);

writeln(MyRecord(int).firstElement(tup):string);
