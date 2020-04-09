var arr1 : [1..20] int;
var arr2 : [1..300, 1..200] int;
var arr3 : [1..10, 1..10, 1..10, 1..10] int; 
var dom : domain(4) = {1..5, 1..4, 1..3, 1..2};

writeln(arr1.size); //20
writeln(arr2.size); //60000
writeln(arr3.size); //10000
writeln(dom.size); //120

//Size (array) sanity checks
if arr1.size == arr1.size {
    writeln("match");
}

if arr2.size == arr2.size {
    writeln("match");
}

if arr3.size == arr3.size {
    writeln("match");
}

//Size (domain) sanity checks
if dom.size == dom.size {
    writeln("match");
} 
