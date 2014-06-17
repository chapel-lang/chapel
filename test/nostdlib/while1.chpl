proc main() {
  var sum = 0;
  var i = 1;
  while i <= 10 {
    sum += i;
    i += 1;
    if i == 3 {
      break;
    }
  }
  var str = sum:string;
  __primitive("chpl_error", str);
}


