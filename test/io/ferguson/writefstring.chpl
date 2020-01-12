use IO;

config type t = string;
writef("%s\n":t, "boo");
writef("%2.1s|\n":t, "boo");
writef("%-2.1s|\n":t, "boo");
if unicodeSupported() {
  writef("%2.2s|%-4.4s|\n":t, "boo", "boo");
  writef("%2.2s|%4.4s|\n":t, "Ａ", "ＡＡ");
}

writef(" 123456789ABCD\n":t);
writef("1%2.1s\n":t, "123456789");
writef("2%2.2s\n":t, "123456789");
writef("3%2.3s\n":t, "123456789");
writef("4%2.4s\n":t, "123456789");
writef("5%2.5s\n":t, "123456789");
writef("6%2.6s\n":t, "123456789");
writef("7%2.7s\n":t, "123456789");
writef("8%2.8s\n":t, "123456789");
writef("9%2.9s\n":t, "123456789");
writef("A%2.10s\n":t, "123456789");
writef("B%2.11s\n":t, "123456789");
writef("C%2.12s\n":t, "123456789");
writef("D%2.13s\n":t, "123456789");

writef(" 123456789ABCD\n":t);
writef("1%2.1js\n":t, "123456789");
writef("2%2.2js\n":t, "123456789");
writef("3%2.3js\n":t, "123456789");
writef("4%2.4js\n":t, "123456789");
writef("5%2.5js\n":t, "123456789");
writef("6%2.6js\n":t, "123456789");
writef("7%2.7js\n":t, "123456789");
writef("8%2.8js\n":t, "123456789");
writef("9%2.9js\n":t, "123456789");
writef("A%2.10js\n":t, "123456789");
writef("B%2.11js\n":t, "123456789");
writef("C%2.12js\n":t, "123456789");
writef("D%2.13js\n":t, "123456789");
