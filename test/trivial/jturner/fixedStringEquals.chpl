pragma "fixed string"
record fixed_length_string {
  param stringLen : int;
  var contents : string;
}

proc fixed_string(param s:string) {
  const x = new fixed_length_string(stringLen = s.length, contents = s);
  return x;
}

proc =(a:fixed_length_string, param b:string) {
  return fixed_string(b);
}

var x:fixed_length_string(5);
x = "sally";
writeln(x);

