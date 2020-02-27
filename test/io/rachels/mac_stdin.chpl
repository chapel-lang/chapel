use IO;

var word: string;
do {
  word = stdin.read(string);
  writeln(word);
} while (word != "/exit");
