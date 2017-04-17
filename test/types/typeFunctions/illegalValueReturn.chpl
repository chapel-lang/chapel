
proc helper(param x : int) type {
  if x == 0 then
    return int;
  else if x == 1 then
    return string;
  else
    return 5;
}

var foo : helper(42);
