proc foo( input_array : [?dom] real ) : ([dom] real,) {
  return (input_array,);
}

proc main(){
  var input_array : [1..10] real = for i in 1..10 do i;
  var t = foo( input_array );
  writeln(t);
}
