var ItGoodFor = "absolutely nothing";

export proc whatisItGoodFor(): c_string {
  return ItGoodFor:c_ptrConst(c_char):c_string;
}

proc deinit() {
  writeln("War, huh, yeah");
}
