use CTypes, Curl, URL, Subprocess, OS.POSIX;

extern {
  #include <curl/curl.h>
}

proc main() throws {
  // Port zero reserves an unused port without touching other tests' servers.
  const script =
    "import http.server\n" +
    "server = http.server.HTTPServer(('127.0.0.1', 0), " +
                                   "http.server.SimpleHTTPRequestHandler)\n" +
    "print(server.server_port, flush=True)\n" +
    "server.serve_forever()\n";
  var server = spawn(["python3", "-u", "-c", script],
                     stdin=pipeStyle.close, stdout=pipeStyle.pipe,
                     stderr=pipeStyle.pipe);
  defer {
    server.sendPosixSignal(SIGTERM);
    server.wait();
  }
  var port: int;
  assert(server.stdout.read(port));
  const url = "http://127.0.0.1:" + port:string + "/test.txt";
  var reader = openUrlReader(url);
  defer reader.close();

  assert(CURLAUTH_ANY.type == c_ulong);
  assert((CURLAUTH_ANYSAFE & CURLAUTH_BASIC) == 0);
  setopt(reader, CURLOPT_HTTPAUTH, CURLAUTH_ANY:c_long);
  var contents: string;
  reader.readAll(contents);
  assert(contents == "this\nis a test\n");
  writeln("URL read with authentication options passed");
}
