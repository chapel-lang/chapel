use LinkedLists;

proc main {
  var lst: LinkedList(int);
  lst.pushBack(1);
  on Locales(1) do
    lst.pushBack(2);
  lst.pushBack(3);

  lst.remove(2);
  writeln(lst);
}
