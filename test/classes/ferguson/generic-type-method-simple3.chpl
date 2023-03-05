record MyRecord {
  type myType;

  proc type getType() type do return this.myType;
}

writeln(MyRecord(int).getType():string);
