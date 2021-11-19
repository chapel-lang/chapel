config type dataType = string;

var sep = " ":dataType;
var my = "my":dataType;
var test = "test":dataType;
var message = "message":dataType;

var arr0 : [1..0] dataType;

var tup3 = (my,test,message);
var tup1 = (my,);
var arr3 = [my,test,message];
var arr1 = [my];

writeln(sep.join(tup3));              // test 3 tuple
writeln(sep.join(tup1));              // test 1 tuple
writeln(sep.join(arr3));              // test 3 element array
writeln(sep.join(arr1));              // test 1 element array
writeln(sep.join(arr0));              // test empty array
writeln(sep.join(my,test,message));   // test varargs
writeln(sep.join(my));                // test single arg
