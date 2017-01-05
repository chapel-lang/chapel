record MyRecord {
  type myType;

  proc type getType() type return myType;
}

writeln(MyRecord(int).getType():string);
