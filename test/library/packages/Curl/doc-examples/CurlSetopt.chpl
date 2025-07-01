//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Curl;
use URL;

use URL;
var curlfile = openUrlReader("http://example.com");
var username = b"foo";
var password = b"bar";

/* START_EXAMPLE_0 */
extern const CURLOPT_USERNAME: CURLoption;
extern const CURLOPT_PASSWORD: CURLoption;

setopt(curlfile, (CURLOPT_USERNAME, username),
                 (CURLOPT_PASSWORD, password));
/* STOP_EXAMPLE_0 */
