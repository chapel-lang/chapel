class IteratorHash : hashable {
  var x: uint;

  iter hash(): uint {
    return x.hash(); // This also triggers an error message
  }
}
