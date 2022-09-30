use IO;

var ch = openreader("./jab.txt");

const line1 = ch.readLine(string);
const line2 = ch.readLine(string);
const remaining = ch.readAll(string);
ch.close();

const s = line1 + line2 + remaining;

write(s);
