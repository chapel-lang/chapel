module M {
  var x: int;
  forwarding x; // internal error
  // forwarding var y: int; // results in the same error
}
