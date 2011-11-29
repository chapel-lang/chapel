/* Convert a Roman number to an Arabic number using string->enum conversions */

enum numeral {I=1, V=5, X=10, L=50, C=100, D=500, M=1000};
config var roman: string = "I";

proc main() {
  var i = 1;
  var sum = 0;

  // Check that all digits are legal
  [i in 1..(roman.length)]
    select roman.substring(i) {
      when "I","V","X","L","C","D","M" do {}
      otherwise halt("Bad digit: ", roman.substring(i));
    }

  do {
    if (i == (roman.length)) {
      sum += roman.substring(i):numeral;
      i += 1;
    } else if (roman.substring(i):numeral < roman.substring(i+1):numeral) {
      /* If the number to the left is lower than to the right,
         subtract it from the number on the right and add to the total */
      sum += roman.substring(i+1):numeral - roman.substring(i):numeral;
      i += 2;
    } else {
      sum += roman.substring(i):numeral;
      i += 1;
    }
  } while i <= (roman.length);

  writeln(roman, " is: ", sum);
}
