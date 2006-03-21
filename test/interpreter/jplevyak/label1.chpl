
pragma "inline" fun foo() {
  var i : int = 0;
  while(i < 10) {
    i = i + 1;
    writeln(i);
    if (i > 5) {
      goto one;
    }
  }
  writeln(i*10);
  label one {}; 
  return;    
}

fun main() {
 foo();
 return;
}

