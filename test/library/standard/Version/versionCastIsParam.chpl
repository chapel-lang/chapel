use Version;

var v = createVersion(1,23,0);
param test = (v:string == "version 1.23.0");
if test == false then
  compilerError("Boo!");

if test == true then
  compilerError("Yay!");


