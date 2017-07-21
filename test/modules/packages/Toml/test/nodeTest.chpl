
use TOML;

config const file: string;

proc main() {
  var tomlChannel = openreader(file);
  var tomlData = parseToml(tomlChannel);
  writeln("Before Mutation: ", tomlData);

  // New Table
  var tblD: domain(string);
  var tbl: [tblD] Toml;

  // Table indexed into and new table added
  tomlData["A.B"]["C"] = new Toml(tbl);

  // Add elements to new table C
  var toAdd: bool = true;
  tomlData["A.B.C"]["new-key-added"] = new Toml(toAdd);

  writeln("After Mutation: ", tomlData);

  // test toString proc
  var strInt: string = tomlData["A.B"]["number"].toString();
  writeln("A.B.number = ",strInt); 

  writeln(); //for spacing

  // write a random table to stdout
  writeln("KV pairs in table A.C");
  writeln(tomlData["A.C"]);
}
