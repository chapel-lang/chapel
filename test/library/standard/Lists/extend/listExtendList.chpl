use Lists;

// Control these via COMPOPTS to test class types and no parallel safety.
config type listType = int;
config param listLock = true;

// Extend a List with the contents of another list.
{
  var lst1 = new list(listType, listLock);

  for i in 1..5 do
    lst1.append(i:listType);

  var lst2 = new list(listType, listLock);
  lst2.extend(lst1);

  writeln(lst1);
  writeln(lst2);
}

