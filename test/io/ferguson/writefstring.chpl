writef("%s\n", "boo");
writef("%2.1s|\n", "boo");
writef("%-2.1s|\n", "boo");
if unicodeSupported() {
  writef("%2.2s|%-4.4s|\n", "boo", "boo");
  writef("%2.2s|%4.4s|\n", "Ａ", "ＡＡ");
}

writef(" 123456789ABCD\n");
writef("1%2.1s\n", "123456789");
writef("2%2.2s\n", "123456789");
writef("3%2.3s\n", "123456789");
writef("4%2.4s\n", "123456789");
writef("5%2.5s\n", "123456789");
writef("6%2.6s\n", "123456789");
writef("7%2.7s\n", "123456789");
writef("8%2.8s\n", "123456789");
writef("9%2.9s\n", "123456789");
writef("A%2.10s\n", "123456789");
writef("B%2.11s\n", "123456789");
writef("C%2.12s\n", "123456789");
writef("D%2.13s\n", "123456789");

writef(" 123456789ABCD\n");
writef("1%2.1js\n", "123456789");
writef("2%2.2js\n", "123456789");
writef("3%2.3js\n", "123456789");
writef("4%2.4js\n", "123456789");
writef("5%2.5js\n", "123456789");
writef("6%2.6js\n", "123456789");
writef("7%2.7js\n", "123456789");
writef("8%2.8js\n", "123456789");
writef("9%2.9js\n", "123456789");
writef("A%2.10js\n", "123456789");
writef("B%2.11js\n", "123456789");
writef("C%2.12js\n", "123456789");
writef("D%2.13js\n", "123456789");
