{
  var myString = "1234";
  var myInt: int;

  myInt = myString: int;
  myInt = myInt + 1;
  myString = myInt: string;
  
  writeln(myInt);
  writeln(myString);
}

{
  var myString = "1234";

  myString = ((myString: int) + 1): string;
  writeln(myString);
}
