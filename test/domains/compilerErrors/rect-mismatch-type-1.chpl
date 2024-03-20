// related: test/arrays/deitz/part5/test_array_uint2.chpl

var DnsStencDom = {-1:int(8)..1:int(8), -1:int(8)..1:int(8)};
DnsStencDom = (-1..0, -1..0); // error: int8 <- int64
