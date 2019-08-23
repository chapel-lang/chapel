
private use Lists;
use MasonUpdate;
	
use FileSystem;
	
config const pattern = "";
	
proc main() {
  var args: list(string);
  args.append("mason");
  args.append("update");
  for arg in pattern.split() {
    if arg != "" then args.append(arg);
  }	
	updateRegistry('Mason.toml', args);
}