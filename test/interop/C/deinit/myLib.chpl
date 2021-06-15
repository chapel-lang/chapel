var ItGoodFor = "absolutely nothing";

export proc whatisItGoodFor(): c_string {
  return ItGoodFor.c_str();
}

proc deinit() {
  writeln("War, huh, yeah");
}
