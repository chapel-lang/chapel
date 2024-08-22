use JSON;

class Fighter {
  var subclass:string;
  var level:int;
}

var s = "{\"subclass\":\"Ninja\", \"level\":7}";
var n = fromJson(s, owned Fighter);
writeln(n);
