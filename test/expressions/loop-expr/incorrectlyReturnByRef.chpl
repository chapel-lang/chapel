const arr = [1,2,3];
const cond = true;
proc foo() const ref {
  if cond then
    return arr;
  else {
    return [0..#3] 1;
  }
}
foo();
