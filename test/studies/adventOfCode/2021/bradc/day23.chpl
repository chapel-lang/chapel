use List, Map;

// STOPPING POINT: Why do I keep pruning the same configuration so many
// times?  There can't be that many ways to get into it, can there?
//
// THOUGHT: nothing prevents me from doing bad move A then B and later
// doing bad move B then A.

config const practice = true;
config const thresh = 100;

const slotsPerRoom=2;
const numRooms=4;
const numMonsters=numRooms*slotsPerRoom;

const Rooms = {0..<numRooms},
      Spaces = {0..<slotsPerRoom};

const inf = max(int);

// #############
// #         --#
// #---------11#
// #12345678901#
// ###1#5#9#D###
//   #2#6#A#E#
//   #3#7#B#F#
//   #4#8#C#G#

const distance: [1..11] [1..numMonsters] int = [
                            /*  1 */  [3, 4, 5, 6, 7, 8, 9, 10],
                            /*  2 */  [2, 3, 4, 5, 6, 7, 8, 9],
                            /*  3 */  [inf, inf, inf, inf, inf, inf, inf, inf],
                            /*  4 */  [2, 3, 2, 3, 4, 5, 6, 7],
                            /*  5 */  [inf, inf, inf, inf, inf, inf, inf, inf],
                            /*  6 */  [4, 5, 2, 3, 2, 3, 4, 5],
                            /*  7 */  [inf, inf, inf, inf, inf, inf, inf, inf],
                            /*  8 */  [6, 7, 4, 5, 2, 3, 2, 3],
                            /*  9 */  [inf, inf, inf, inf, inf, inf, inf, inf],
                            /* 10 */  [8, 9, 6, 7, 4, 5, 2, 3],
                            /* 11 */  [9, 10, 7, 8, 5, 6, 3, 4]];

var searchedConfigs: map(int, int);

record path {
  var steps: list(int);
  proc init() {
  }

  proc init(xs:int...) {
    this.complete();
    for x in xs do
      steps.pushBack(x);
  }
}

// If I were cleverer or less lazy, I'd probably compute this rather than
// hard-coding it...
var Path: [1..11,1..numMonsters] path;
Path[1,1] = new path(-3,-2,-1);
Path[1,2] = new path(1,-3,-2,-1);
Path[1,3] = new path(-5,-4,-3,-2,-1);
Path[1,4] = new path(3,-5,-4,-3,-2,-1);
Path[1,5] = new path(-7,-6,-5,-4,-3,-2,-1);
Path[1,6] = new path(5,-7,-6,-5,-4,-3,-2,-1);
Path[1,7] = new path(-9,-8,-7,-6,-5,-4,-3,-2,-1);
Path[1,8] = new path(7,-9,-8,-7,-6,-5,-4,-3,-2,-1);

Path[2,1] = new path(-3,-2);
Path[2,2] = new path(1,-3,-2);
Path[2,3] = new path(-5,-4,-3,-2);
Path[2,4] = new path(3,-5,-4,-3,-2);
Path[2,5] = new path(-7,-6,-5,-4,-3,-2);
Path[2,6] = new path(5,-7,-6,-5,-4,-3,-2);
Path[2,7] = new path(-9,-8,-7,-6,-5,-4,-3,-2);
Path[2,8] = new path(7,-9,-8,-7,-6,-5,-4,-3,-2);

Path[4,1] = new path(-3,-4);
Path[4,2] = new path(1,-3,-4);
Path[4,3] = new path(-5,-4);
Path[4,4] = new path(3,-5,-4);
Path[4,5] = new path(-7,-6,-5,-4);
Path[4,6] = new path(5,-7,-6,-5,-4);
Path[4,7] = new path(-9,-8,-7,-6,-5,-4);
Path[4,8] = new path(7,-9,-8,-7,-6,-5,-4);

Path[6,1] = new path(-3,-4,-5,-6);
Path[6,2] = new path(1,-3,-4,-5,-6);
Path[6,3] = new path(-5,-6);
Path[6,4] = new path(3,-5,-6);
Path[6,5] = new path(-7,-6);
Path[6,6] = new path(5,-7,-6);
Path[6,7] = new path(-9,-8,-7,-6);
Path[6,8] = new path(7,-9,-8,-7,-6);

Path[8,1] = new path(-3,-4,-5,-6,-7,-8);
Path[8,2] = new path(1,-3,-4,-5,-6,-7,-8);
Path[8,3] = new path(-5,-6,-7,-8);
Path[8,4] = new path(3,-5,-6,-7,-8);
Path[8,5] = new path(-7,-8);
Path[8,6] = new path(5,-7,-8);
Path[8,7] = new path(-9,-8);
Path[8,8] = new path(7,-9,-8);

Path[10,1] = new path(-3,-4,-5,-6,-7,-8,-9,-10);
Path[10,2] = new path(1,-3,-4,-5,-6,-7,-8,-9,-10);
Path[10,3] = new path(-5,-6,-7,-8,-9,-10);
Path[10,4] = new path(3,-5,-6,-7,-8,-9,-10);
Path[10,5] = new path(-7,-8,-9,-10);
Path[10,6] = new path(5,-7,-8,-9,-10);
Path[10,7] = new path(-9,-10);
Path[10,8] = new path(7,-9,-10);

Path[11,1] = new path(-3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,2] = new path(1,-3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,3] = new path(-5,-6,-7,-8,-9,-10,-11);
Path[11,4] = new path(3,-5,-6,-7,-8,-9,-10,-11);
Path[11,5] = new path(-7,-8,-9,-10,-11);
Path[11,6] = new path(5,-7,-8,-9,-10,-11);
Path[11,7] = new path(-9,-10,-11);
Path[11,8] = new path(7,-9,-10,-11);

forall (i,j) in Path.domain do
  if (Path[i,j].steps.size != distance[i][j] &&
      (Path[i,j].steps.size != 0 || distance[i][j] != inf)) then
    writeln((i,j), ": ", Path[i,j].steps, " != ", distance[i][j]);

var bestScore = max(int);

param A1 = 1,
      A2 = 2,
      B1 = 3,
      B2 = 4,
      C1 = 5,
      C2 = 6,
      D1 = 7,
      D2 = 8;

record state {
  var hallway: [1..11] int;
  var room: [Rooms] [Spaces] int = if practice
    then [[B1:int, A1:int], [C1:int, D2:int], [B2:int, C2:int], [D1:int, A2:int]]
    else [[A1:int, C2:int], [D1:int, D2:int], [A2:int, B1:int], [C1:int, B2:int]];
  var monsterloc: [1..numMonsters] int;
  var cost: int;

  proc init() {
    this.complete();
    for r in Rooms do
      for s in Spaces do
        monsterloc[room[r][s]] = r*slotsPerRoom + s + 1;
//    writeln("monsterloc = ", monsterloc);
  }

  proc uniqueID() {
    var id = 0;
    for i in hallway.domain do
      if i != 3 && i != 5 && i != 7 && i != 9 {
        id <<= 4;
        id |= hallway[i];
      }
    for i in 1..numRooms*slotsPerRoom {
      id <<= 2;
      const occupant = room[(i-1)/slotsPerRoom][(i-1)%slotsPerRoom];
//      writeln("checking room ", i, " with occupant ", occupant);
      if occupant then
        if isTargetLoc(i, occupant) {
//          writeln("Matched");
          id |= 0b11;
        } else {
//          writeln("Didn't match");
          id |= 0b1;
        }
//       writeIDAsBits();
    }
/*
    writeln("ID for: ");
    print();
    writeln("is:");
    writeIDAsBits();
*/

    return id;

    proc writeIDAsBits() {
      var mask = 0x1 << (28+16-1);
      for i in 1..7 {
        for j in 1..4 {
//          writeln("\nmask is: ", mask);
          write(if id & mask then "1" else "0");
          mask >>= 1;
        }
        write("|");
      }
      for i in 1..8 {
        for j in 1..2 {
//          writeln("\nmask is: ", mask);
          write(if id & mask then "1" else "0");
          mask >>= 1;
        }
        write("|");
      }
      writeln();
    }
  }

  proc print() {
    writeln("############# ", cost);

    write("#");
    for i in hallway do
      write(intToMonsterString(i));
    writeln("#");

    write("##");
    for r in Rooms do
      write("#", intToMonsterString(room[r][0]));
    writeln("###");

    for i in 1..<slotsPerRoom {
      write("  ");
      for r in Rooms do
        write("#", intToMonsterString(room[r][i]));
      writeln("#");
    }

    writeln("  #########");
  }

  proc locOpen(loc: int) {
    if loc < 0 then
      return hallway[-loc] == 0;
    else
      return room[(loc-1)/slotsPerRoom][(loc-1)%slotsPerRoom] == 0;
  }

  proc pathClear(hallway: int, room: int) {
    if distance[hallway][room] == inf then
      return false;
    for step in Path[hallway, room].steps do
       if !locOpen(step) then
         return false;
    return true;
  }

  proc locToRoomSlot(loc) {
    const room = (loc-1)/slotsPerRoom;
    const slot = (loc-1)%slotsPerRoom;
    return (room, slot);
  }

  proc isTargetLoc(currentLoc, monster) {
    if currentLoc < 0 then
      return false;
    // are we in an OK spot?
    const (roomID, slot) = locToRoomSlot(currentLoc);
//    writeln("We're in ", (roomID, slot));
    const monsterType = (monster-1)/slotsPerRoom;
//    writeln("And our type is ", monsterType);
    const inOKspot = monsterType == roomID;
    if inOKspot {
//      writeln("So we might be OK");
      // if so, let's make sure we're not locking anyone other than our partner
      // in
      for i in slot+1..<slotsPerRoom {
        const occupantType = (room[roomID][i]-1)/slotsPerRoom;
        if occupantType != monsterType then {
//          writeln("Except that there's a monster of type ", occupantType, " in slot ", i);
          return false;
        }
      }
    }
    return inOKspot;
  }

  iter monsterCanMove(m: int) {
    const currentLoc = monsterloc[m];
//    writeln("Seeing if monster ", m, " can move from ", currentLoc);
    if currentLoc > 0 {
      // TODO: didn't make this nested in original program... OOPs
      if !isTargetLoc(currentLoc, m) {
//        writeln("It would like to leave its room");
        for h in 1..11 do
          if pathClear(hallway=h, room=currentLoc) then
            yield (currentLoc, -h, distance[h][currentLoc]);
      }
    } else {
      const targetRoom = (m-1) / slotsPerRoom;
      for i in Spaces by -1 {
//        writeln("i = ", i);
        if room[targetRoom][i] == 0 {
          const targetSpace = targetRoom*slotsPerRoom + i + 1;
//          print();
//          writeln("I'd like to move to slot ", i, " of my room (I'm ", m);
          hallway[-currentLoc] = 0;
          if pathClear(hallway=-currentLoc, room=targetSpace) {
            yield (currentLoc, targetSpace, distance[-currentLoc][targetSpace]);
            hallway[-currentLoc] = m;
            return;
          } else {
//            writeln("...but the path is not clear");
            hallway[-currentLoc] = m;
            return;
          }
        } else if (room[targetRoom][i]-1)/slotsPerRoom == targetRoom {
//          writeln("Slot ", i, " already has a friend in it");
          // OK, someone good is already here, let's go on
        } else {
//          writeln("Slot ", i, " has an enemy in it");
          break;
        }
      }
    }
  }

  proc addMonsterToLoc(loc, monster) {
    if loc < 0 then
      hallway[-loc] = monster;
    else
      room[(loc-1)/slotsPerRoom][(loc-1)%slotsPerRoom] = monster;
  }

  proc removeMonsterFromLoc(loc) {
    addMonsterToLoc(loc, 0);
  }

  proc success() {
    return && reduce [(m,i) in zip(monsterloc, 1..numMonsters)] isTargetLoc(m,i);
  }

  proc optimisticPath(loc) {
    const occupant = hallway[loc];
    if occupant == 0 then
      return 0;
    else
      return (distance[loc][occupant-(occupant%slotsPerRoom == 0)]*(10**((occupant-1)/slotsPerRoom)));
  }
  

  proc optimisticCost() {
    return cost + (+ reduce for i in hallway.domain do optimisticPath(i));
  }

  proc simulate(in depth = 0) {
    if success() {
      writeln("*** Found a winning combination with score: ", cost);
      if cost < bestScore then
        bestScore = cost;
    } else {
      for monster in 1..numMonsters {
        for (src,dst,pathLength) in monsterCanMove(monster) {
          var newBoard = this;
//          writeln("monster ", monster, " can move from ", src, " to ", dst);
          newBoard.monsterloc[monster] = dst;
          newBoard.addMonsterToLoc(dst, monster);
          newBoard.removeMonsterFromLoc(src);
          newBoard.cost += (pathLength * 10**((monster-1)/slotsPerRoom));
//          newBoard.print();
//          writeln("depth = ", depth);
          // prune boards that are already too expensive
/*
          if newBoard.optimisticCost() < bestScore {
            writeln("This will never win:\n");
            newBoard.print();
          }
*/

// size = 113100 without newBoard.cost logic here
// size = 183000 with just newBoard.cost logic here
// size =  67900 with both newBoard.cost and newBoard.optimisticCost logic here

       if newBoard.cost < bestScore && newBoard.optimisticCost() < bestScore {
          const id = newBoard.uniqueID();
          var alreadyDidThis = false;
          if searchedConfigs.contains(id) {
            const prevCost = searchedConfigs[id];
            if prevCost < newBoard.cost {
              alreadyDidThis = true;
            } else {
              searchedConfigs.replace(id, newBoard.cost);
            }
          } else {
            searchedConfigs.add(id, newBoard.cost);
/*
            if searchedConfigs.size%100 == 0 then
              writeln(searchedConfigs.size);
*/
          }

/*
          const alreadyDidThis = searchedConfigs.contains(id);
          if alreadyDidThis {
//            writeln("We've seen this config before:");
//            newBoard.print();
          } else {
            searchedConfigs.add(id);
          }
*/
            

          if !alreadyDidThis then
            newBoard.simulate(depth+1);
/*
          else {
            if (newBoard.cost >= bestScore && newBoard.cost - bestScore < thresh) {
              writeln("--> pruning a score of ", newBoard.cost, " <--");
              newBoard.print();
            }
          }
*/
        }
        }
      }
    }
  }
}

proc intToMonsterString(x: int) {
  if x == 0 then
    return ".";
  const targetRoom = (x-1)/slotsPerRoom;
  select targetRoom {
  when 0 do return "A";
  when 1 do return "B";
  when 2 do return "C";
  when 3 do return "D";
  }
  halt("Whoa");
}

var board: state;
board.print();
board.simulate();
writeln("Best score was: ", bestScore);
