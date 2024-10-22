module UnusedTypeQuery {

  proc foo(ref a: ?t) {
    a = 1;
  }
  proc bar(ref arr: [?d] ?elt) where elt == int{
    arr[1] = 1;
  }

}
