use CTypes;
var A = [c"hi", c"there", c"everybody"];
var B = ["hi".c_str(), "there".c_str(), "everybody".c_str()];

writeln(A.type:string);

writeln(B.type:string);
