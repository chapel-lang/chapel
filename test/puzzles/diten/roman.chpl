/* Convert a Roman number to an Arabic number using string->enum conversions */

enum numeral {I=1, V=5, X=10, L=50, C=100, D=500, M=1000};
config var roman: string = "I";

proc main() {
  var i = 1;
  var sum = 0;

  // Check that all digits are legal
  [i in 1..(roman.size)]
    select roman[i] {
      when "I","V","X","L","C","D","M" do {}
      otherwise halt("Bad digit: ", roman[i]);
    }

  do {
    if (i == (roman.size)) {
      sum += roman[i]:numeral:int;
      i += 1;
    } else if (roman[i]:numeral:int < roman[i+1]:numeral:int) {
      /* If the number to the left is lower than to the right,
         subtract it from the number on the right and add to the total */
      sum += roman[i+1]:numeral:int - roman[i]:numeral:int;
      i += 2;
    } else {
      sum += roman[i]:numeral:int;
      i += 1;
    }
  } while i <= (roman.size);

  writeln(roman, " is: ", sum);
}
