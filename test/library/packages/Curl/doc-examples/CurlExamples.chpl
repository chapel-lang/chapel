//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Curl;
use URL;

{
/* START_EXAMPLE_0 */
use URL;
var urlreader = openUrlReader("http://example.com");
var str: bytes;
// Output each line read from the URL to stdout
while urlreader.readLine(str) {
  write(str);
}
/* STOP_EXAMPLE_0 */

var concat: bytes;
urlreader = openUrlReader("http://example.com");
while urlreader.readLine(str) do concat += str;
assert(!concat.isEmpty());

writeln('~@OK0');
}

// Note that this constant is described as optional but has to exist for
// the test to compile...
{
/* START_EXAMPLE_1 */
extern const CURLOPT_VERBOSE: CURLoption;
/* STOP_EXAMPLE_1 */

writeln('~@OK1');
}

{
/* START_EXAMPLE_2 */
// This example changes the Curl options before connecting
use URL, Curl;

var reader = openUrlReader("http://example.com");
var str: bytes;
// Set verbose output from curl
extern const CURLOPT_VERBOSE: CURLoption;
Curl.setopt(reader, CURLOPT_VERBOSE, true);

// now read into the bytes
reader.readAll(str);
writeln(str);
reader.close();
/* STOP_EXAMPLE_2 */

assert(!str.isEmpty());
writeln('~@OK2');
}

{
/* START_EXAMPLE_3 */
// This example uses the curl_easy_ interface from libcurl
// to POST some json data.
use CTypes;
import Curl;

extern const CURLOPT_CUSTOMREQUEST: Curl.CURLoption;
extern const CURLOPT_HTTPHEADER: Curl.CURLoption;
extern const CURLOPT_POSTFIELDS: Curl.CURLoption;
extern const CURLOPT_URL: Curl.CURLoption;
extern const CURLOPT_WRITEFUNCTION: Curl.CURLoption;

// Called with the contents of the server's response; does nothing with it.
// Else libcurl writes it to stdout.
proc null_write_callback(ptr: c_ptr(c_char), size: c_size_t, nmemb: c_size_t, userdata: c_ptr(void)) {
  return size * nmemb;
}

var curl = Curl.curl_easy_init();

var args = new Curl.slist();
args.append("Accept: application/json");
args.append("Content-Type: application/json");
Curl.curl_easy_setopt(curl, CURLOPT_HTTPHEADER, args);

var jsonPayload = '{"foo": "bar"}';
Curl.curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload);

Curl.curl_easy_setopt(curl, CURLOPT_URL, 'http://localhost:3000/posts');
Curl.curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, 'POST');
Curl.curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, c_ptrTo(null_write_callback):c_ptr(void));

var ret = Curl.curl_easy_perform(curl);

args.free();
Curl.curl_easy_cleanup(curl);
/* STOP_EXAMPLE_3 */
writeln('~@OK3');
}
