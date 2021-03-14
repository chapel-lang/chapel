proc main(){
  import Sort.sort;

  var size : uint(32) = 3;
  var array : [1..size] real = [3,1,2];

  sort(array);
  writeln(array);
}
