var hello = ("Hello,", "World!");
writeln('"', ' '.join(hello), '"');
writeln('"', '+++'.join(hello), '"');

var fox = ("The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog");
writeln('"', ' '.join(fox), '"');
writeln('"', ','.join(fox), '"');

var nums = ("1", "2", "3", "4", "5");
writeln('"', "abcdef".join(nums), '"');
writeln('"', "".join(nums), '"');

var kinda_empty = ("1", "", "", "", "", "2");
writeln('"', ",".join(kinda_empty), '"');
writeln('"', "".join(kinda_empty), '"');

var empty = ("", "", "", "", "", "");
writeln('"', ",".join(empty), '"');
writeln('"', "".join(empty), '"');
