use Version;

var v = new versionValue(1,23,0);
param test = ("version " + v:string == "version 1.23.0");

var v2 = new versionValue(1,23,0,"vass");
compilerWarning(v2:string);

if test == false then
  compilerError("Boo!");

if test == true then
  compilerError("Yay!");
