writeln('"', ' '.join("Hello,", "World!"), '"');
writeln('"', '+++'.join("Hello,", "World!"), '"');

writeln('"', ' '.join("The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"), '"');
writeln('"', ','.join("The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"), '"');

writeln('"', "abcdef".join("1", "2", "3", "4", "5"), '"');
writeln('"', "".join("1", "2", "3", "4", "5"), '"');

writeln('"', ",".join("1", "", "", "", "", "2"), '"');
writeln('"', "".join("1", "", "", "", "", "2"), '"');

writeln('"', ",".join("", "", "", "", "", ""), '"');
writeln('"', "".join("", "", "", "", "", ""), '"');
