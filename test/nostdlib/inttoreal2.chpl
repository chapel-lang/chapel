proc main() {
  var x = 12345678; 
  var y= x:real / 2147483647;
  var str = y:string;
  __primitive("chpl_error", str);
}

