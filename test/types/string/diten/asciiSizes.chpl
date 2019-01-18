param param_word = "hello";
var var_word = param_word;

const pascii = ascii(param_word);
const vascii = ascii(var_word);

type ptype = pascii.type;
type vtype = vascii.type;

writeln((pascii, vascii));

if ptype != vtype {
  writeln("param/var types don't match");
  writeln("param: ", ptype:string);
  writeln("  var: ", vtype:string);
}
