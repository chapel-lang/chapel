const fi = openMemFile();
const ch = fi.writer();
ch.close();
ch.write("hi");
