extern proc chpl_nodeName(): c_string;
extern var chpl_nodeID: chpl_nodeID_t;

var locale_name = here.chpl_name();

if locale_name == chpl_nodeName():string then
  writeln("matches nodeName");
else if locale_name == chpl_nodeName():string + "-" + chpl_nodeID:string then
  writeln("matches nodeName-nodeID");
else
  writeln("unknown format: ", locale_name);
