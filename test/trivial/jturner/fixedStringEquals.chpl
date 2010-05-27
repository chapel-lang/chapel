pragma "fixed string"
record fixed_length_string {
  param stringLen : int;
  var contents : string;
}

def fixed_string(param s:string) {
  const x = new fixed_length_string(stringLen = length(s), contents = s);
  return x;
}

def =(a:fixed_length_string, param b:string) {
  return fixed_string(b);
}

var x:fixed_length_string(5) = "sally";
writeln(x);

