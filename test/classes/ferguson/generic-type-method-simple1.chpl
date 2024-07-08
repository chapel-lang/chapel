record MyRecord {
  type myType;

  proc type getType() type do return myType;
}

writeln(MyRecord(int).getType():string);
