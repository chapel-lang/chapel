use Curl;

// Open a URL and get a reader on a section of the site
config const url = "http://127.0.0.1:8000/2701-h.htm";
//= "https://www.gutenberg.org/files/2701/2701-h/2701-h.htm";
//var f = openurl("http://127.0.0.1:8000/test.txt");
//var f = openurl("http://www.example.com");
var f = openurl(url);
var reader = f.reader();

var str:string;

var nlines = 0;
// While we can read a line from example.com, write it to 'out.txt'
while(reader.readline(str)) {
  stdout.write(str);
  nlines += 1;
}

writeln("FOUND ", nlines, " lines");
