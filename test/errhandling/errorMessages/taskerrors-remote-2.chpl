
var te: owned TaskErrors?;

on Locales[1] do te = new owned TaskErrors(new unmanaged Error("hi"));

writeln(te!.first());
