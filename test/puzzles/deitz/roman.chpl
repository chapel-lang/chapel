use IO;

var s: string;

write("Enter a roman numeral: ");
read(s);

var romanNumerals: domain(string);
romanNumerals += "I";
romanNumerals += "V";
romanNumerals += "X";
romanNumerals += "L";
romanNumerals += "C";
romanNumerals += "D";
romanNumerals += "M";

var arabicNumerals: [romanNumerals] int;
arabicNumerals("I") = 1;
arabicNumerals("V") = 5;
arabicNumerals("X") = 10;
arabicNumerals("L") = 50;
arabicNumerals("C") = 100;
arabicNumerals("D") = 500;
arabicNumerals("M") = 1000;

var n, current, last: int;;
last = 0;
for i in 0..<s.size {
  current = arabicNumerals(s[i]);
  if current > last then
    current -= last;
  else if current == last then
    current += last;
  else
    n += last;
  last = current;
}
n += current;
writeln(n);
