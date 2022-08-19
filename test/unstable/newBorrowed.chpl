class C { var x: int = 0; }

proc test() {
  for i in 1..8 do var c = (new owned C(i)).borrow();
  return;
}
test();

