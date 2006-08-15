/* testing the funality of indexing into sequences */
def main() {
  var sequ1 : seq of int = (/ 5 /); 
  var sequ2 : seq of int = (/ 4, 18 /);
  var sequ4 : seq of int =  (/ 1, 2, 3, 4, 5, 6, 7 /);
  
  /* 1 element sequences */ 
  var index_int : int = sequ1(1);
  writeln("sequ1(1) is ", index_int);
  
  /* 2 element sequences */
  index_int = sequ2(2);
  writeln("sequ2(2) is ", index_int);

 
  /* multiple element sequences */
  index_int = sequ4(1);
  writeln("sequ4(1) is ", index_int);
  index_int =  sequ4(4);
  writeln("sequ4(4) is ", index_int);
  index_int =  sequ4(7);
  writeln("sequ4(7) is ", index_int);
  write("sequ4(8) is ");   
  index_int = sequ4(8);  
}
