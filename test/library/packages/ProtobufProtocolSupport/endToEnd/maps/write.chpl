
use maps;
use IO;

var file = open("out", iomode.cw);
var writingChannel = file.writer();

var messageObj = new mapTest();

var tmpObj:testA;
tmpObj.bo = true;
messageObj.mapfield1[1:int(32)] = tmpObj;

messageObj.mapfield2[1:int(32)] = "chapel";
messageObj.mapfield2[2:int(32)] = "protobuf";

messageObj.serialize(writingChannel);
