use IO;

var ch = openreader("./jab.txt");

const line1 = ch.readLine(bytes);
const line2 = ch.readLine(bytes);
const remaining = ch.readAll(bytes);
ch.close();

const b = line1 + line2 + remaining;

write(b);
