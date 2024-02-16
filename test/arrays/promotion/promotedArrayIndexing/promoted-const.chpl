use TestArrayConst;

//
// The following are all not potential races, just using the promoted array index is ok
//

{
  // use free function
  proc print(a: int) do writeln(a);
  print(ArrayOfInt[ArrayOfIndices]);
}

{
  // use method
  ArrayOfRecord[ArrayOfIndices].print();
}

{
  // use operator
  var C = ArrayOfInt[ArrayOfIndices] + 3;
  writeln(C);
}


//
// The following are all potential races
//

{
  // assignment
  ArrayOfInt[ArrayOfIndices] = 3;
}

{
  // modify with free function
  proc addTwo(ref a: int) do a += 2;
  addTwo(ArrayOfInt[ArrayOfIndices]);
}

{
  // modify with method
  ArrayOfRecord[ArrayOfIndices].addOne();
}

{
  // modify with operator
  ArrayOfInt[ArrayOfIndices] += 3;
  ArrayOfInt.localAccess[ArrayOfIndices] += 5;
}
