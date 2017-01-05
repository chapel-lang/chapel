record MyRecord {
  type myType;

  proc type getType() type return this.type.myType;
}

writeln(MyRecord(int).getType():string);
