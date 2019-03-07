record MyRecord {
  type myType;

  proc type getType() type return this.myType;
}

writeln(MyRecord(int).getType():string);
