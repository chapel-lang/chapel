use Subprocess;
use IO;
var sub = spawn(["cat","moby.txt"],stdout=PIPE);

var line:bytes;
var st:iostyle;

sub.stdout._set_style(style = new iostyle(binary=1));
sub.stdout.readbytes(line); 
write(line);

sub.wait();
