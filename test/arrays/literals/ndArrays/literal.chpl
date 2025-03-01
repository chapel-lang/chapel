
{
  var arr3DReal32 : [1..3, 0..2, 10..12] real(32) = [
    1.10, 1.20, 1.30 ;
    2.10, 2.20, 2.30 ;
    3.10, 3.20, 3.30 ;
    ;
    1.11, 1.21, 1.31 ;
    2.11, 2.21, 2.31 ;
    3.11, 3.21, 3.31 ;
    ;
    1.12, 1.22, 1.32 ;
    2.12, 2.22, 2.32 ;
    3.12, 3.22, 3.32 ;
  ]:real(32);
  printArray(arr3DReal32, "arr3DReal32");
}
{
  var arr2d = [1, 2; 3, 4];
  printArray(arr2d, "arr2d");
}
{
 var arr3d = [1, 2; 3, 4;; 5, 6; 7, 8];
  printArray(arr3d, "arr3d"); 
}
{
  var arr4d: [{11..12, -1..1, 1..3, 0..3}] int = [
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
  printArray(arr4d, "arr4d");
}
{
  var arr5d: [{0..2, 0..1, 0..2, 0..2, 0..1}] real = [
    1.10, 1.20 ;
    2.10, 2.20 ;
    3.10, 3.20 ;
    ;
    1.11, 1.21 ;
    2.11, 2.21 ;
    3.11, 3.21 ;
    ;
    1.12, 1.22 ;
    2.12, 2.22 ;
    3.12, 3.22 ;
    ;;
    1.13, 1.23 ;
    2.13, 2.23 ;
    3.13, 3.23 ;
    ;
    1.14, 1.24 ;
    2.14, 2.24 ;
    3.14, 3.24 ;
    ;
    1.15, 1.25 ;
    2.15, 2.25 ;
    3.15, 3.25 ;
    ;;;
    1.10, 1.20 ;
    2.10, 2.20 ;
    3.10, 3.20 ;
    ;
    1.11, 1.21 ;
    2.11, 2.21 ;
    3.11, 3.21 ;
    ;
    1.12, 1.22 ;
    2.12, 2.22 ;
    3.12, 3.22 ;
    ;;
    1.13, 1.23 ;
    2.13, 2.23 ;
    3.13, 3.23 ;
    ;
    1.14, 1.24 ;
    2.14, 2.24 ;
    3.14, 3.24 ;
    ;
    1.15, 1.25 ;
    2.15, 2.25 ;
    3.15, 3.25 ;
    ;;;
    1.10, 1.20 ;
    2.10, 2.20 ;
    3.10, 3.20 ;
    ;
    1.11, 1.21 ;
    2.11, 2.21 ;
    3.11, 3.21 ;
    ;
    1.12, 1.22 ;
    2.12, 2.22 ;
    3.12, 3.22 ;
    ;;
    1.13, 1.23 ;
    2.13, 2.23 ;
    3.13, 3.23 ;
    ;
    1.14, 1.24 ;
    2.14, 2.24 ;
    3.14, 3.24 ;
    ;
    1.15, 1.25 ;
    2.15, 2.25 ;
    3.15, 3.25 ;
   ];
   printArray(arr5d, "arr5d");
}
{
  var arr1d = [1, 2, 3];
  var nested2d = [arr1d, arr1d; arr1d, arr1d];
  printArray(nested2d, "nested2d");
}
{
  var nested2d = [[1, 2], [3, 4]; [5, 6], [7, 8]];
  printArray(nested2d, "nested2dLiteral");
}
{
  var columnVec = [1; 2; 3];
  printArray(columnVec, "columnVec");
}
{
  var rowVec = [1, 2, 3;];
  printArray(rowVec, "rowVec");
}
{
  var trailing = [1; 2; 3;];
  printArray(trailing, "trailingSemi");
}
{
  var trailing = [1,; 2; 3,;];
  printArray(trailing, "trailingCommaSemi");
}
{
  var trailing = [1, 2, 3,;1,2,3,;];
  printArray(trailing, "trailingCommaSemi2");
}
{
  var trailing = [1; 2; 3,];
  printArray(trailing, "trailingComma");
}
{
  var arr2d = [1, 2; 3, 4;];
  var nested2d = [arr2d, arr2d; arr2d, arr2d];
  printArray(nested2d, "nested2dLiteral2");
}
{
  var arr2d = [1, 2; 3, 4;];
  var nested2d = [arr2d, arr2d, arr2d, arr2d];
  printArray(nested2d, "nested2dLiteral3");
}
{
  var singleElm = [1;];
  printArray(singleElm, "singleElm");
}

proc printArray(arr, name="") {
  if name != "" {
    writeln(name, ":");
  }
  writeln(arr.type:string, arr.domain, arr.domain.shape, arr, sep="\n");
  writeln("=====================================================");
}
