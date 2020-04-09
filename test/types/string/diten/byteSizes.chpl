param param_word = "hello";
var var_word = param_word;

const pascii = param_word.byte(0);
const vascii = var_word.byte(0);

type ptype = pascii.type;
type vtype = vascii.type;

writeln((pascii, vascii));

if ptype != vtype {
  writeln("param/var types don't match");
  writeln("param: ", ptype:string);
  writeln("  var: ", vtype:string);
}
