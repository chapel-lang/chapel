use RunServer;
use Curl;
use DateTime;

proc test1() {
  writeln("\ntest1\n");
  var f = "test.txt";
  var url = "http://" + host + ":" + port + "/" + f;
  var urlreader = downloadUrl(url);

  setopt(urlreader, CURLOPT_VERBOSE, true);

  var str: string;
  while urlreader.readline(str) {
    writeln(str);
  }

  stderr.flush();
  stdout.flush();
}

extern const CURLINFO_FILETIME:c_int;
extern proc curl_easy_getinfo(handle:c_ptr(CURL), info, arg);

proc test2() {
  writeln("\ntest2\n");
  var f = "test.txt";
  var url = "http://" + host + ":" + port + "/" + f;
  var urlreader = downloadUrl(url);

  setopt(urlreader, (CURLOPT_VERBOSE, true),
                    (CURLOPT_FILETIME, true));

  var str: string;
  while urlreader.readline(str) {
    writeln(str);
  }

  var time:c_long = 0;

  curl_easy_getinfo(getCurlHandle(urlreader), CURLINFO_FILETIME,
                    c_ptrTo(time));

  writeln("Remote Time ", datetime.utcfromtimestamp(time));

  stderr.flush();
  stdout.flush();
}

startServer();
test1();
test2();
stopServer();
