proc main() {
  var sum = 0;
  var i = 1;
  do {
    sum = i;
  } while( i > 10 );
  var str = sum:string;
  __primitive("chpl_error", str);
}


