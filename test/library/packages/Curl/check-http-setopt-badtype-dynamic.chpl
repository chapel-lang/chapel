use Curl;

extern const CURLOPT_URL: CURLoption;

var url = "http://notactuallyused.127.0.0.1/nosuchfile";
var curl = curl_easy_init();

// test a type that's invalid for CURLOPT_URL, but is valid for some
// option.  Detected at runtime.
var rc = curl_easy_setopt(curl, CURLOPT_URL, false);

writeln("rc is ", rc);

