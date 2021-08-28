use Curl;

extern const CURLOPT_URL: CURLoption;

var url = "http://notactuallyused.127.0.0.1/nosuchfile";
var curl = curl_easy_init();

// test a type that's not valid for any option, detected at compile time.
var rc = curl_easy_setopt(curl, CURLOPT_URL, (url,));

writeln("rc is ", rc);

