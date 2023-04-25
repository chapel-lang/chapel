use IO;

const f = open("test.txt", ioMode.r);

const locales = f.localesForRegion(0, 10);

f.close();
