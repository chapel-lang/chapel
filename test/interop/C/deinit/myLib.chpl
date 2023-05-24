use CTypes;

var ItGoodFor = "absolutely nothing";

export proc whatisItGoodFor(): c_string {
  return c_ptrToConst_helper(ItGoodFor):c_string;
}

proc deinit() {
  writeln("War, huh, yeah");
}
