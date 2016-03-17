var hello = ["Hello,", "World!"];
var fox = ["The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"];
var nums = ["1", "2", "3", "4", "5"];
var kinda_empty = ["1", "", "", "", "", "2"];
var empty = ["", "", "", "", "", ""];

on Locales[numLocales-1] {
  writeln('"', ' '.join(hello), '"');
  writeln('"', '+++'.join(hello), '"');

  writeln('"', ' '.join(fox), '"');
  writeln('"', ','.join(fox), '"');

  writeln('"', "abcdef".join(nums), '"');
  writeln('"', "".join(nums), '"');

  writeln('"', ",".join(kinda_empty), '"');
  writeln('"', "".join(kinda_empty), '"');

  writeln('"', ",".join(empty), '"');
  writeln('"', "".join(empty), '"');
}
