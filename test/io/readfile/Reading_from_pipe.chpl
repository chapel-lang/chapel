use Subprocess;
use IO;
var sub = spawn(["echo","How are you?"],stdout=PIPE);

var line:bytes;
var st:iostyle;

sub.stdout._set_style(style = new iostyle(binary=1));
while sub.stdout.readbytes(line) {
  write(line);
  break;
}

sub.wait();
