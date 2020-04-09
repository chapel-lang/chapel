// Test that the relational operators are folded over enums
enum color { red=2, orange=5, yellow, green=5, blue=6, indigo=1, violet };
enum nums { one=1, two, three, four, five, six };
proc main() {
  var result3 = color.red != nums.two;
  var result5 = color.blue > nums.six;
  var result6 = color.red > nums.three;
  var result7 = color.blue > nums.five;
  var result9 = color.indigo >= nums.four;
  var result11 = nums.one < color.red;
  var result12 = nums.one < color.indigo;
  var result14 = color.orange <= nums.six;
  var result15 = color.violet <= nums.two;
  var result16 = color.orange <= nums.one;
}
