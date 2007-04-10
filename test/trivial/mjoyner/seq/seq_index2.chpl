/* testing the funality of indexing into multiple types of sequences */
def main() {
  var sequ1 : seq of string = _seq( "hard", "working" ); 
  var sequ2 : seq of int =  _seq( 1, 2, 3, 4, 5, 6, 7 );
  
  /* element access of multiple types of sequences */ 
  var index_string : string = sequ1(2);
  writeln("sequ1(2) is ", index_string);
  var index_int : int = sequ2(4);
  writeln("sequ2(4) is ", index_int);
}
