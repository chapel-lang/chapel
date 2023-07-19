use CTypes;

var ItGoodFor = "absolutely nothing";

export proc whatisItGoodFor(): c_ptrConst(c_char) {
  return ItGoodFor.c_str();
}

proc deinit() {
  writeln("War, huh, yeah");
}
