proc return_arrays() {
  // some size defining thing only known at runtime in function
  var magic_constant : int = 5;
  var arr : [0..<magic_constant] int = 0;
  return (arr, arr);
}

var arr1, arr2;
(arr1, arr2) = return_arrays();

writeln(arr1);
writeln(arr2);
