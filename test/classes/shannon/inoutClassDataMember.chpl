class volcano {
  var alertLevel: int;
}

proc setVolcano(inout al: int, inout name: string) {
  name = "Mount St. Helens";
  al = 2;
}

var mtStHelens: owned volcano = new owned volcano();
var name: string;

setVolcano(mtStHelens.alertLevel, name);

writeln("Name: ", name);
writeln("Alert Level: ", mtStHelens.alertLevel);
