param s1 = "hello";
param s2 = "world";
param s3 = "hello";

param b01 = !(s1 == s2);
param b02 =   s1 == s3;
param b03 =   s1 != s2;
param b04 = !(s1 != s3);
param b05 = !(s1 > s2);
param b06 = !(s1 > s3);
param b07 =   s2 > s1;
param b08 =   s1 < s2;
param b09 = !(s1 < s3);
param b10 = !(s2 < s1);
param b11 = !(s1 >= s2);
param b12 =   s1 >= s3;
param b13 =   s2 >= s1;
param b14 =   s1 <= s2;
param b15 =   s1 <= s3;
param b16 = !(s2 <= s1);


param final = b01 && b02 && b03 && b04 && b05 && b06 && b07 && b08 &&
              b09 && b10 && b11 && b12 && b13 && b14 && b15 && b16;

if !final then
  writeln("b01=", b01, " b02=", b02, " b03=", b03, " b04=", b04, " ",
          "b05=", b05, " b06=", b06, " b07=", b07, " b08=", b08, " ",
          "b09=", b09, " b10=", b10, " b11=", b11, " b12=", b12, " ",
          "b13=", b13, " b14=", b14, " b15=", b15, " b16=", b16);

writeln(final);
