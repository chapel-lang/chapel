pragma "safe"
module MutateRefBorrow {

class MyClass {
  var x:int;
}


/*(The question is, if you mutated a ref to a borrow, would  the other borrow
  checking rules catch an error, or would it cause a gap in the checking?)
 */
proc bad1(ref r:borrowed MyClass) {

  var owny = new owned MyClass(1);
  r = owny.borrow();
}

proc test() {
  var myowned = new owned MyClass(1);

  var borrow = myowned.borrow();
  bad1(borrow);
}

test();

}
