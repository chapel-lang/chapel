use IO, List;

config const debug = false;

var Caves: domain(string);
var Corridors: [Caves] list(string);

var line: string;
while readLine(line) {
  var pair = line.strip().split("-");
  Caves += pair[0];
  Caves += pair[1];
  Corridors[pair[0]].pushBack(pair[1]);
  Corridors[pair[1]].pushBack(pair[0]);
}

if debug {
  forall (n,e) in zip(Caves, Corridors) do
    writeln(n, "-", e);
  writeln();
}

var numPaths: atomic int;
var Visited: [Caves] bool;
var path: list(string);
findAllPaths("start", Visited, path, false);
writeln(numPaths.read());

proc findAllPaths(room, in Visited, in path, in visitedSmall: bool) {
  if room == "end" {
    if debug then
      writeln(path);
    numPaths.add(1);
  } else {
    path.pushBack(room);
    Visited[room] = true;
    for/*all*/ adjRoom in Corridors[room] {
      if debug then
        writeln("Doing adjRoom: ", adjRoom);
      var myVisitedSmall = visitedSmall;
      var proceed = true;
      if adjRoom.isLower() {
        if adjRoom == "start" {
          proceed  = false;
        } else {
          if Visited[adjRoom] then
            if myVisitedSmall {
              proceed = false;
            } else {
              proceed = true;
              myVisitedSmall = true;
            }
        }
      }
      if proceed then {
        if debug then
          writeln("Proceeding");
        findAllPaths(adjRoom, Visited, path, myVisitedSmall);
      } else {
        if debug then
          writeln("skipping");
      }
    }
  }
}
