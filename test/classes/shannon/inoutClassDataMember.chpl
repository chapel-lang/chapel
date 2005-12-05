class volcano {
  var alertLevel: integer;
}

function setVolcano(inout al: integer, inout name: string) {
  name = "Mount St. Helens";
  al = 2;
}

var mtStHelens: volcano = volcano();
var name: string;

setVolcano(mtStHelens.alertLevel, name);

writeln("Name: ", name);
writeln("Alert Level: ", mtStHelens.alertLevel);

