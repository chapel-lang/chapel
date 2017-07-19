
use TOML;

config const file: string;

proc main() { 
  var tomlFile = parseToml(file);
  writeln("Before Mutation: ", tomlFile);

  // New Table
  var tblD: domain(string);
  var tbl: [tblD] Toml;

  // Table indexed into and new table added
  tomlFile.getIdx("A.B")["C"] = new Toml(tbl);

  // Add elements to new table C
  var toAdd: bool = true;
  tomlFile.getIdx("A.B.C")["new-key-added"] = new Toml(toAdd);

  writeln("After Mutation: ", tomlFile);

  // test toString proc
  var strInt: string = tomlFile.getIdx("A.B")["number"].toString();
  writeln("A.B.number = ",strInt); 

  writeln(); //for spacing

  // write a random table to stdout
  writeln("KV pairs in table A.C");
  writeln(tomlFile.getIdx("A.C"));
}