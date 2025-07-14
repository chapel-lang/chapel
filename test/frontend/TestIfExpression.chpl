use Print;

proc main() {
  var flag = true;
  var x = if flag then 1 else 0;
  println(x);

  param p = true;
  var y = 5;
  y = if p then x else 3;
  println(y);
}
