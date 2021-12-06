config const myString = "test";
config const myBytes = b"test";

on Locales[1] {
  writeln(myBytes);
  writeln(myBytes == b"test");
  writeln(myBytes.locale_id);

  writeln(myString);
  writeln(myString == "test");
  writeln(myString.locale_id);
}
