proc writelnSortedByDom(arr) {
  var first = true;
  for i in arr.domain.sorted() {
    if first then first = false; else write(" ");
    write(arr(i));
  }
  writeln();
}


proc writelnSorted(arr) {
  var first = true;
  for elem in arr.sorted() {
    if first then first = false; else write(" ");
    write(elem);
  }
  writeln();
}

proc writelnSorted(dom: domain) {
  var first = true;
  for elem in dom.sorted() {
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
