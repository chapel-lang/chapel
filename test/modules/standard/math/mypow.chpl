proc pow(a: real, b: real) : real{
  return a**b;
}


proc main(){
  var i : real;
  var j : real;
  i = 3.1;
  j = 2;
  writeln("pow func ", 3.4*pow(i,j));       // THIS NOT
}
