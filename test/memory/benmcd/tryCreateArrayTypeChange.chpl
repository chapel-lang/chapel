var arr: [1..10] bool = false;

var switched = arr.domain.tryCreateArray(int, arr);

writeln(arr);
writeln(switched);
writeln(arr.type:string);
writeln(switched.type:string);
