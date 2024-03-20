
var te: owned TaskErrors?;

on Locales[1] do te = new owned TaskErrors(new unmanaged Error("hi"));

for e in te! do
  writeln(e);
