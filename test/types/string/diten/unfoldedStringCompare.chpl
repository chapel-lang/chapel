var s1 = "hello";
var s2 = "world";
var s3 = "hello";

var b01 = !(s1 == s2);
var b02 =   s1 == s3;
var b03 =   s1 != s2;
var b04 = !(s1 != s3);
var b05 = !(s1 > s2);
var b06 = !(s1 > s3);
var b07 =   s2 > s1;
var b08 =   s1 < s2;
var b09 = !(s1 < s3);
var b10 = !(s2 < s1);
var b11 = !(s1 >= s2);
var b12 =   s1 >= s3;
var b13 =   s2 >= s1;
var b14 =   s1 <= s2;
var b15 =   s1 <= s3;
var b16 = !(s2 <= s1);


var final = b01 && b02 && b03 && b04 && b05 && b06 && b07 && b08 &&
            b09 && b10 && b11 && b12 && b13 && b14 && b15 && b16;
if !final then
  writeln("b01=", b01, " b02=", b02, " b03=", b03, " b04=", b04, " ",
          "b05=", b05, " b06=", b06, " b07=", b07, " b08=", b08, " ",
          "b09=", b09, " b10=", b10, " b11=", b11, " b12=", b12, " ",
          "b13=", b13, " b14=", b14, " b15=", b15, " b16=", b16);

writeln(final);
