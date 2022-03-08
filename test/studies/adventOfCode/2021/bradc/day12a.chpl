use IO, List;

config const debug = false;

var Caves: domain(string);
var Corridors: [Caves] list(string);

var line: string;
while readline(line) {
  var pair = line.strip().split("-");
  Caves += pair[0];
  Caves += pair[1];
  Corridors[pair[0]].append(pair[1]);
  Corridors[pair[1]].append(pair[0]);
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
  if (room == "end") {
//    writeln(path);
    numPaths.add(1);
  } else {
    path.append(room);
    Visited[room] = true;
    for/*all*/ adjRoom in Corridors[room] {
//      writeln("Doing adjRoom: ", adjRoom);
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
//        writeln("Proceeding");
        findAllPaths(adjRoom, Visited, path, myVisitedSmall);
      } else {
//        writeln("skipping");
      }
    }
/*
    for/*all*/ adjRoom in Corridors[room] {
      var myVisitedSmall = visitedSmall;
      var proceed = true;
      if adjRoom.isLower() {
        writeln("Doing adjRoom: ", adjRoom);
        if myVisitedSmall || adjRoom == "start" {
          writeln("In then");
          if Visited[adjRoom] || adjRoom == "start" then {
            proceed = false;
            writeln("Not proceeding");
          }
        } else {
          writeln("In else");
          myVisitedSmall = true;
          Visited[room] = false;
        }
      }
      
      if proceed then {
        writeln("Proceeding from ", path, " to ", adjRoom);
        findAllPaths(adjRoom, Visited, path, myVisitedSmall);
      }
    }
*/
    }
}
