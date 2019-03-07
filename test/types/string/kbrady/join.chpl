var hello = ["Hello,", "World!"];
var fox = ["The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"];
var summer = ["Été", "Élongé"];
var nums = ["1", "2", "3", "4", "5"];
var kinda_empty = ["1", "", "", "", "", "2"];
var empty = ["", "", "", "", "", ""];

writeln('"', ' '.join(hello), '"');
writeln('"', '+++'.join(hello), '"');

writeln('"', ' '.join(fox), '"');
writeln('"', ','.join(fox), '"');

writeln('"', ' '.join(summer), '"');
writeln('"', ','.join(summer), '"');
writeln('"', '€€€'.join(summer), '"');

writeln('"', "abcdef".join(nums), '"');
writeln('"', "àêíôùŷ".join(nums), '"');
writeln('"', "".join(nums), '"');

writeln('"', ",".join(kinda_empty), '"');
writeln('"', "".join(kinda_empty), '"');

writeln('"', ",".join(empty), '"');
writeln('"', "".join(empty), '"');
