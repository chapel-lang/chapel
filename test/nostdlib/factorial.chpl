

proc factorial(x:int):int {
  if x <= 1 then return 1;
  else return x*factorial(x-1);
}


proc main() {
  var six = factorial(3);
  var str = six:string;
  __primitive("chpl_error", str);
}

