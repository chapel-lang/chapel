var s1 : string;
var s2 = "123456";

writeln(s1.size);

if s1.size == s1.length {
    writeln("match");
}

writeln(s2.size);

if s1.size == s1.length {
  writeln("match");  
}

s1 = s2;

if( s1.size == s2.length && s1.length == s2.size ){
    writeln("match");
}
