use IO, TOML;

config const f: string;

proc main() {
  var tomlChannel = openreader(f);
  var tomlData = parseToml(tomlChannel);
  writeln("Before Mutation: ", tomlData);

  // New Table
  var tblD: domain(string);
  var tbl: [tblD] unmanaged Toml?;

  // Table indexed into and new table added
  tomlData["A.B"]!.set("C", tbl);

  // Add elements to new table C
  var toAdd: bool = true;
  tomlData["A.B.C"]!.set("new-key-added", toAdd);

  writeln("After Mutation: ", tomlData);

  // test toString proc
  var strInt: string = tomlData["A.B"]!["number"]!.toString();
  writeln("A.B.number = ",strInt); 

  writeln(); //for spacing

  // write a random table to stdout
  writeln("KV pairs in table A.C");
  writeln(tomlData["A.C"]);

 
  // Test of the "copy constructor"
  // New Toml
  var tbl2D: domain(string);
  var tbl2: [tbl2D] unmanaged Toml?;
  var tomlData2 = new unmanaged Toml(tbl2);

  // copy Toml A.B in tomlData to Toml A in TomlData2
  tomlData2["A"] = new unmanaged Toml(tomlData["A"]!);
  writeln(tomlData["A"]);
  writeln("Should be the same as");
  writeln(tomlData2["A"]);

  delete tomlData2;  
  delete tomlData;
  tomlChannel.close();
}
