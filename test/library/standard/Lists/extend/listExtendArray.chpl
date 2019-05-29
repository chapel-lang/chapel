use Lists;

// Control these via COMPOPTS to test class types and no parallel safety.
config type listType = int;
config param listLock = true;

// Extend the same List with the contents of an array.
{
  var lst1 = new List(listType, listLock);
  var arr1: [1..5] listType = [6, 7, 8, 9, 10];

  lst1.extend(arr1);

  writeln(lst1);
}

