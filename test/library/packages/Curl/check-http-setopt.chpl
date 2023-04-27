// This test verifies that Curl + QIO integration
// allows Curl setopt calls before the transfer starts.

module CheckHttpSetOpt {

  use RunServer;
  use URL;
  use Curl;
  use Time;

  extern const CURLOPT_VERBOSE: CURLoption;
  extern const CURLOPT_FILETIME: CURLoption;
  extern const CURLOPT_URL: CURLoption;
  extern const CURLOPT_WRITEFUNCTION: CURLoption;
  extern const CURLINFO_FILETIME: CURLINFO;

  proc test1() throws {
    writeln("\ntest1\n");
    var f = "test.txt";
    var url = "http://" + host + ":" + port + "/" + f;
    var urlreader = openUrlReader(url);

    setopt(urlreader, CURLOPT_VERBOSE, true);

    var str: string;
    while urlreader.readLine(str) {
      writeln(str);
    }

    stderr.flush();
    stdout.flush();
  }


  proc test2() throws {
    writeln("\ntest2\n");
    var f = "test.txt";
    var url = "http://" + host + ":" + port + "/" + f;
    var urlreader = openUrlReader(url);

    setopt(urlreader, (CURLOPT_VERBOSE, true),
	              (CURLOPT_FILETIME, true));

    var str: string;
    while urlreader.readLine(str) {
      writeln(str);
    }

    var time:c_long = 0;

    curl_easy_getinfo(getCurlHandle(urlreader), CURLINFO_FILETIME,
		      c_ptrTo(time));

    writeln("Remote Time ", dateTime.createUtcFromTimestamp(time));

    stderr.flush();
    stdout.flush();
  }

  // Test a string-type option, CURLOPT_URL
  proc test3() throws {
    writeln("\ntest3\n");
    var f = "test.txt";
    var url = "http://" + host + ":" + port + "/" + f;
    var urlreader = openUrlReader(""); // set real url via setopt()

    setopt(urlreader, CURLOPT_VERBOSE, true);
    setopt(urlreader, CURLOPT_URL, url);

    var str: string;
    while urlreader.readLine(str) {
      writeln(str);
    }

    stderr.flush();
    stdout.flush();
  }

  // Test a bytes-type option, CURLOPT_URL
  proc test4() throws {
    writeln("\ntest4\n");
    var f = "test.txt";
    var url = "http://" + host + ":" + port + "/" + f;
    var urlreader = openUrlReader(""); // set real url via setopt()

    setopt(urlreader, CURLOPT_VERBOSE, true);
    setopt(urlreader, CURLOPT_URL, url:bytes);

    var str: string;
    while urlreader.readLine(str) {
      writeln(str);
    }

    stderr.flush();
    stdout.flush();
  }

  // Test a string-type option, CURLOPT_URL, via the libcurl-based API
  proc write_callback(ptr: c_ptr(c_char), size: c_size_t,
		      nmemb: c_size_t, userdata: c_void_ptr) {
    writeln("callback called");
    var str = try! createStringWithBorrowedBuffer(ptr:c_string,
                                                  (size * nmemb):int);
    write(str);
    return size * nmemb;
  }

  proc test5() throws {
    writeln("\ntest5\n");
    var f = "test.txt";
    var url = "http://" + host + ":" + port + "/" + f;
    var curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
		     c_ptrTo(write_callback):c_void_ptr);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    stderr.flush();
    stdout.flush();
  }

  proc main() throws {
    startServer();
    defer stopServer();
    test1();
    test2();
    test3();
    test4();
    test5();
  }
}
