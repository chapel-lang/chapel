
/*****
      ex_arrayop.chpl -
      Examples of operations on arrays.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

var dom1 = {1..3, 4..6};
var arr1 : [dom1] int;
var arr2 : [dom1] int;
var arr3 : [dom1] int;

writeln("\nDemonstrating array operations");

writeln("arr1 = 3");
arr1 = 3;
writeln(arr1);

writeln("arr1 = arr1 + 5");
arr1 += 5;
writeln(arr1);

arr2(1,4) = 1;   arr2(1,5) = 2;   arr2(1,6) = 4;
arr2(2,4) = 2;   arr2(2,5) = 4;   arr2(2,6) = 8;
arr2(3,4) = 1;   arr2(3,5) = 8;   arr2(3,6) = 2;
writeln("arr2");
writeln(arr2);

writeln("arr1 / arr2");
arr3 = arr1 / arr2;
writeln(arr3);

writeln("arr3 from for");
arr3 = for (x, y) in arr3.domain do x+y;
writeln(arr3);
