require "printArr.h";

extern proc printfArr(arr: [] c_double, const ref len: c_int = (arr.size): c_int);

var arr = [32.0, 212.0, 98.6, 0.0, 100.0];

printfArr(arr);
