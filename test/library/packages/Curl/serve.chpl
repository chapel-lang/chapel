use Spawn;
use Curl;

config const port = 8000;

// Start a little HTTP server
var sub = spawn(["python3", "-m", "http.server",
                 port:string, "--bind", "127.0.0.1"]);

var url = "http://127.0.0.1:" + port:string + "/test.txt";

var reader = openurl(url).reader();

var str: string;
while(reader.readline(str)) do
  stdout.write(str);

// Kill the little HTTP server
sub.send_signal(SIGINT);
