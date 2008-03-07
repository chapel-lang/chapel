def writelnSortedByDom(arr) {
  var first = true;
  for i in arr.domain.sorted() {
    if first then first = false; else write(" ");
    write(arr(i));
  }
  writeln();
}


def writelnSorted(arr) {
  var first = true;
  for elem in arr.sorted() {
    if first then first = false; else write(" ");
    write(elem);
  }
  writeln();
}

def writelnSorted(dom: domain) {
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
