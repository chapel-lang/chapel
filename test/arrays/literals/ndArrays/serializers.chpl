use IO, ChplFormat;

var arr1d = [1, 2, 3];
var arr2d = [1, 2, 3; 4, 5, 6; 7, 8, 9];
var arr3d = [1, 2, 3;
             4, 5, 6;
             ;
             7, 8, 9;
             10, 11, 12;
             ;
             13, 14, 15;
             16, 17, 18;];
var arr4d = [
  1, 2, 3, 4 ;
  5, 6, 7, 8 ;
  9, 10, 11, 12 ;
  ;
  13, 14, 15, 16 ;
  17, 18, 19, 20 ;
  21, 22, 23, 24 ;
  ;
  25, 26, 27, 28 ;
  29, 30, 31, 32 ;
  33, 34, 35, 36 ;
  ;;
  37, 38, 39, 40 ;
  41, 42, 43, 44 ;
  45, 46, 47, 48 ;
  ;
  49, 50, 51, 52 ;
  53, 54, 55, 56 ;
  57, 58, 59, 60 ;
  ;
  61, 62, 63, 64 ;
  65, 66, 67, 68 ;
  69, 70, 71, 72 ;
];

var vec1 = [1, 2, 3;];
var vec2 = [1; 2; 3];
var singleArr = [1;];

writeln("printing arrays using the default serializer");
printArray(arr1d, "arr1d", stdout);
printArray(arr2d, "arr2d", stdout);
printArray(arr3d, "arr3d", stdout);
printArray(arr4d, "arr4d", stdout);
printArray(vec1, "vec1", stdout);
printArray(vec2, "vec2", stdout);
printArray(singleArr, "singleArr", stdout);

writeln("printing arrays using the chplSerializer");
{
  var w = stdout.withSerializer(new chplSerializer());
  printArray(arr1d, "arr1d", w);
  printArray(arr2d, "arr2d", w);
  printArray(arr3d, "arr3d", w);
  printArray(arr4d, "arr4d", w);
  printArray(vec1, "vec1", w);
  printArray(vec2, "vec2", w);
  printArray(singleArr, "singleArr", w);
}

proc printArray(arr, name, writer) {
  writeln("printing ", name, " of type ", arr.type:string);

  write("%?: ");
  writer.writef("%?\n", arr);

  write("%{?}: ");
  writer.writef("%{?}\n", arr);
  
  writeln("=====================================================");
}
