proc intToEnglish(n: int): string {
  const ones: [1..19] string = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"];

  const tens: [1..9] string = ["ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"];

  if n <= 0 || n >= 100 then
    halt("I only understand two positive digits, not ", n);

  if n < 20 then
    return ones[n];
  else {
    if n % 10 == 0 then
      return tens[n/10];
    else
      return tens[n / 10] + "-" + intToEnglish(n % 10);
  }
}
