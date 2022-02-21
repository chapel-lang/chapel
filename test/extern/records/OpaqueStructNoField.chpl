require "mydata.h";
require "mydata.c";

use CTypes;


extern "struct mydata" record mydata { }

extern proc getNewData(): c_ptr(mydata);
extern proc acceptData(d:c_ptr(mydata)):void;

proc main() {
  var d = getNewData();
  acceptData(d);
}
