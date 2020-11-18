extern proc chpl_nodeName(): c_string;

var locale_name = here.chpl_name();

if locale_name == createStringWithNewBuffer(chpl_nodeName()) then
  writeln("matches nodeName");
else if locale_name == createStringWithNewBuffer(chpl_nodeName()) + "-" + chpl_nodeID:string then
  writeln("matches nodeName-nodeID");
else
  writeln("unknown format: ", locale_name);
