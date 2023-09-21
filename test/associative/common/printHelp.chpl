use Sort;

proc writelnSortedByDom(arr) {
  var first = true;
  for i in sorted(arr.domain) {
    if first then first = false; else write(" ");
    write(arr(i));
  }
  writeln();
}


proc writelnSorted(arr) {
  var first = true;
  for elem in sorted(arr) {
    if first then first = false; else write(" ");
    write(elem);
  }
  writeln();
}

proc writelnSorted(dom: domain(?)) {
  var first = true;
  for elem in sorted(dom) {
    if first then {
      first = false;
      write("[");
    } else {
      write(", ");
    }
    write(elem);
  }
  writeln("]");
}
