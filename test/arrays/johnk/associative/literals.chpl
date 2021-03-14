var StringDom = {"four", "five", "six"};
var IntDom = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23};

var string2int : [StringDom] int = ["four"=>4, "five"=>5, "six"=>6];

var int2string : [IntDom] string = 
    [1=>"one", 2=>"two", 3=>"three", 5=>"five", 7=>"seven",
     11=>"eleven", 13=>"thirteen", 17=>"seventeen", 19=>"nineteen",
     23=>"twenty-three"];

writeln("A  String => Int Map");
for keyval in zip(string2int.domain, string2int) do
    writeln("A ", keyval);

writeln("B  Sparse Int => String Map");
for keyval in zip(int2string.domain, int2string) do
    writeln("B ", keyval);
