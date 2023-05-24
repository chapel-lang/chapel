use CTypes;
var A = [c"hi", c"there", c"everybody"];
var B = [c_ptrToConst_helper("hi"), c_ptrToConst_helper("there"), c_ptrToConst_helper("everybody")];

writeln(A.type:string);

writeln(B.type:string);
