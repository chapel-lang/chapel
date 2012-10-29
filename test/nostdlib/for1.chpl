proc main() {
  var sum = 0;
  for i in 1..10 {
    sum += i;
    if i == 3 then break;
  }
  var str = sum:string;
  __primitive("chpl_error", str);
}


