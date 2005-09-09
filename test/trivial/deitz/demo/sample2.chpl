config var phrase : string = "shuffle me please";
const n = length(phrase);

var encoded = phrase(1..n by 2) + phrase(2..n by 2);

var decoder : seq of integer;
for i in 1..n/2 do
  decoder = decoder # (/ i, n/2 + n mod 2 + i /);
if n mod 2 == 1 { 
  var tmp : integer = n / 2 + 1;
  decoder = decoder # (/ tmp /);
}
var decoded : string;
for i in decoder do
  decoded = decoded + encoded(i);

writeln("phrase:   ", phrase);
writeln("encoded:  ", encoded);
writeln("decoder:  ", decoder);
writeln("decoded:  ", decoded);
