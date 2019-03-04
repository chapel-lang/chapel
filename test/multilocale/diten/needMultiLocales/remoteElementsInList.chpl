use LinkedLists;

proc main {
  var lst: linkedList(int);
  lst.append(1);
  on Locales(1) do
    lst.append(2);
  lst.append(3);

  lst.remove(2);
  writeln(lst);
}
