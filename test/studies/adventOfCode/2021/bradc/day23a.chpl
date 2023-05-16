use List, Map;

// TODO: extend pruning of "known to be too expensive" paths to include
// fastest paths for monsters at home to get to first place in their
// target room as well.

// STOPPING POINT: Why do I keep pruning the same configuration so many
// times?  There can't be that many ways to get into it, can there?
//
// THOUGHT: nothing prevents me from doing bad move A then B and later
// doing bad move B then A.

config const practice = true;
config const thresh = 100;
config const debug = false;

const slotsPerRoom=4;
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

const distance: [1..11] [1..16] int = [
            /*  1 */  [3, 4, 5, 6,  5, 6, 7, 8,  7, 8, 9, 10,  9, 10, 11, 12],
            /*  2 */  [2, 3, 4, 5,  4, 5, 6, 7,  6, 7, 8,  9,  8,  9, 10, 11],
            /*  3 */  [inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf],
            /*  4 */  [2, 3, 4, 5,  2, 3, 4, 5,  4, 5, 6, 7,   6, 7, 8, 9],
            /*  5 */  [inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf],
            /*  6 */  [4, 5, 6, 7,  2, 3, 4, 5,  2, 3, 4, 5,   4, 5, 6, 7],
            /*  7 */  [inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf],
            /*  8 */  [6, 7, 8, 9,  4, 5, 6, 7,  2, 3, 4, 5,   2, 3, 4, 5],
            /*  9 */  [inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf, inf],
            /* 10 */  [8,  9, 10, 11,  6, 7, 8,  9,  4, 5, 6, 7,  2, 3, 4, 5],
            /* 11 */  [9, 10, 11, 12,  7, 8, 9, 10,  5, 6, 7, 8,  3, 4, 5, 6]];

var searchedConfigs: map((int,int), int);


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
var Path: [1..11,1..16] path;
Path[1,1] = new path(      -3,-2,-1);
Path[1,2] = new path(    1,-3,-2,-1);
Path[1,3] = new path(  2,1,-3,-2,-1);
Path[1,4] = new path(3,2,1,-3,-2,-1);
Path[1,5] = new path(      -5,-4,-3,-2,-1);
Path[1,6] = new path(    5,-5,-4,-3,-2,-1);
Path[1,7] = new path(  6,5,-5,-4,-3,-2,-1);
Path[1,8] = new path(7,6,5,-5,-4,-3,-2,-1);
Path[1,9] = new path(        -7,-6,-5,-4,-3,-2,-1);
Path[1,10] = new path(      9,-7,-6,-5,-4,-3,-2,-1);
Path[1,11] = new path(   10,9,-7,-6,-5,-4,-3,-2,-1);
Path[1,12] = new path(11,10,9,-7,-6,-5,-4,-3,-2,-1);
Path[1,13] = new path(         -9,-8,-7,-6,-5,-4,-3,-2,-1);
Path[1,14] = new path(      13,-9,-8,-7,-6,-5,-4,-3,-2,-1);
Path[1,15] = new path(   14,13,-9,-8,-7,-6,-5,-4,-3,-2,-1);
Path[1,16] = new path(15,14,13,-9,-8,-7,-6,-5,-4,-3,-2,-1);

Path[2,1] = new path(      -3,-2);
Path[2,2] = new path(    1,-3,-2);
Path[2,3] = new path(  2,1,-3,-2);
Path[2,4] = new path(3,2,1,-3,-2);
Path[2,5] = new path(      -5,-4,-3,-2);
Path[2,6] = new path(    5,-5,-4,-3,-2);
Path[2,7] = new path(  6,5,-5,-4,-3,-2);
Path[2,8] = new path(7,6,5,-5,-4,-3,-2);
Path[2,9] = new path(         -7,-6,-5,-4,-3,-2);
Path[2,10] = new path(      9,-7,-6,-5,-4,-3,-2);
Path[2,11] = new path(   10,9,-7,-6,-5,-4,-3,-2);
Path[2,12] = new path(11,10,9,-7,-6,-5,-4,-3,-2);
Path[2,13] = new path(         -9,-8,-7,-6,-5,-4,-3,-2);
Path[2,14] = new path(      13,-9,-8,-7,-6,-5,-4,-3,-2);
Path[2,15] = new path(   14,13,-9,-8,-7,-6,-5,-4,-3,-2);
Path[2,16] = new path(15,14,13,-9,-8,-7,-6,-5,-4,-3,-2);

Path[4,1] = new path(      -3,-4);
Path[4,2] = new path(    1,-3,-4);
Path[4,3] = new path(  2,1,-3,-4);
Path[4,4] = new path(3,2,1,-3,-4);
Path[4,5] = new path(      -5,-4);
Path[4,6] = new path(    5,-5,-4);
Path[4,7] = new path(  6,5,-5,-4);
Path[4,8] = new path(7,6,5,-5,-4);
Path[4,9] = new path(         -7,-6,-5,-4);
Path[4,10] = new path(      9,-7,-6,-5,-4);
Path[4,11] = new path(   10,9,-7,-6,-5,-4);
Path[4,12] = new path(11,10,9,-7,-6,-5,-4);
Path[4,13] = new path(         -9,-8,-7,-6,-5,-4);
Path[4,14] = new path(      13,-9,-8,-7,-6,-5,-4);
Path[4,15] = new path(   14,13,-9,-8,-7,-6,-5,-4);
Path[4,16] = new path(15,14,13,-9,-8,-7,-6,-5,-4);

Path[6,1] = new path(      -3,-4,-5,-6);
Path[6,2] = new path(    1,-3,-4,-5,-6);
Path[6,3] = new path(  2,1,-3,-4,-5,-6);
Path[6,4] = new path(3,2,1,-3,-4,-5,-6);
Path[6,5] = new path(      -5,-6);
Path[6,6] = new path(    5,-5,-6);
Path[6,7] = new path(  6,5,-5,-6);
Path[6,8] = new path(7,6,5,-5,-6);
Path[6,9] = new path(         -7,-6);
Path[6,10] = new path(      9,-7,-6);
Path[6,11] = new path(   10,9,-7,-6);
Path[6,12] = new path(11,10,9,-7,-6);
Path[6,13] = new path(         -9,-8,-7,-6);
Path[6,14] = new path(      13,-9,-8,-7,-6);
Path[6,15] = new path(   14,13,-9,-8,-7,-6);
Path[6,16] = new path(15,14,13,-9,-8,-7,-6);

Path[8,1] = new path(      -3,-4,-5,-6,-7,-8);
Path[8,2] = new path(    1,-3,-4,-5,-6,-7,-8);
Path[8,3] = new path(  2,1,-3,-4,-5,-6,-7,-8);
Path[8,4] = new path(3,2,1,-3,-4,-5,-6,-7,-8);
Path[8,5] = new path(      -5,-6,-7,-8);
Path[8,6] = new path(    5,-5,-6,-7,-8);
Path[8,7] = new path(  6,5,-5,-6,-7,-8);
Path[8,8] = new path(7,6,5,-5,-6,-7,-8);
Path[8,9] = new path(         -7,-8);
Path[8,10] = new path(      9,-7,-8);
Path[8,11] = new path(   10,9,-7,-8);
Path[8,12] = new path(11,10,9,-7,-8);
Path[8,13] = new path(         -9,-8);
Path[8,14] = new path(      13,-9,-8);
Path[8,15] = new path(   14,13,-9,-8);
Path[8,16] = new path(15,14,13,-9,-8);

Path[10,1] = new path(      -3,-4,-5,-6,-7,-8,-9,-10);
Path[10,2] = new path(    1,-3,-4,-5,-6,-7,-8,-9,-10);
Path[10,3] = new path(  2,1,-3,-4,-5,-6,-7,-8,-9,-10);
Path[10,4] = new path(3,2,1,-3,-4,-5,-6,-7,-8,-9,-10);
Path[10,5] = new path(      -5,-6,-7,-8,-9,-10);
Path[10,6] = new path(    5,-5,-6,-7,-8,-9,-10);
Path[10,7] = new path(  6,5,-5,-6,-7,-8,-9,-10);
Path[10,8] = new path(7,6,5,-5,-6,-7,-8,-9,-10);
Path[10,9] = new path(         -7,-8,-9,-10);
Path[10,10] = new path(      9,-7,-8,-9,-10);
Path[10,11] = new path(   10,9,-7,-8,-9,-10);
Path[10,12] = new path(11,10,9,-7,-8,-9,-10);
Path[10,13] = new path(         -9,-10);
Path[10,14] = new path(      13,-9,-10);
Path[10,15] = new path(   14,13,-9,-10);
Path[10,16] = new path(15,14,13,-9,-10);

Path[11,1] = new path(      -3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,2] = new path(    1,-3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,3] = new path(  2,1,-3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,4] = new path(3,2,1,-3,-4,-5,-6,-7,-8,-9,-10,-11);
Path[11,5] = new path(      -5,-6,-7,-8,-9,-10,-11);
Path[11,6] = new path(    5,-5,-6,-7,-8,-9,-10,-11);
Path[11,7] = new path(  6,5,-5,-6,-7,-8,-9,-10,-11);
Path[11,8] = new path(7,6,5,-5,-6,-7,-8,-9,-10,-11);
Path[11,9] = new path(         -7,-8,-9,-10,-11);
Path[11,10] = new path(      9,-7,-8,-9,-10,-11);
Path[11,11] = new path(   10,9,-7,-8,-9,-10,-11);
Path[11,12] = new path(11,10,9,-7,-8,-9,-10,-11);
Path[11,13] = new path(         -9,-10,-11);
Path[11,14] = new path(      13,-9,-10,-11);
Path[11,15] = new path(   14,13,-9,-10,-11);
Path[11,16] = new path(15,14,13,-9,-10,-11);

forall (i,j) in Path.domain do
  if (Path[i,j].steps.size != distance[i][j] &&
      (Path[i,j].steps.size != 0 || distance[i][j] != inf)) then
    writeln((i,j), ": ", Path[i,j].steps, " != ", distance[i][j]);

/*
for (i,j) in Path.domain {
  writeln("Path from ", i, " to ", j);
  var board: state;
  board.hallway = 0;
//  board.room = 0;  // BUG: Can't iterate over values of type int(64)
  forall r in board.room do
    forall s in r do
      s = 0;
  for step in Path[i,j].steps {
    if step < 0 then
      board.hallway[-step] = 1;
    else {
      var (roomID, slot) = board.locToRoomSlot(step);
      board.room[roomID][slot] = 1;
    }
  }
  board.print();
}
*/

var bestScore = max(int);

param A1 = 1,
      A2 = 2,
      A3 = 3,
      A4 = 4,
      B1 = 5,
      B2 = 6,
      B3 = 7,
      B4 = 8,
      C1 = 9,
      C2 = 10,
      C3 = 11,
      C4 = 12,
      D1 = 13,
      D2 = 14,
      D3 = 15,
      D4 = 16;

record state {
  var hallway: [1..11] int;
  var room: [Rooms] [Spaces] int = if practice
    then [[B1:int, D2:int, D3:int, A4:int], [C1:int, C2:int, B4:int, D4:int], [B2:int, B3:int, A2:int, C4:int], [D1:int, A1:int, C3:int, A3:int]]
    else [[A1:int, D2:int, D3:int, C4:int], [D1:int, C3:int, B2:int, D4:int], [A2:int, B1:int, A4:int, B3:int], [C1:int, A3:int, C2:int, B4:int]];
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
        id <<= 5;
        id |= hallway[i];
      }
    var id2 = 0;
    for i in 1..numRooms*slotsPerRoom {
      id2 <<= 2;
      const occupant = room[(i-1)/slotsPerRoom][(i-1)%slotsPerRoom];
//      writeln("checking room ", i, " with occupant ", occupant);
      if occupant then
        if isTargetLoc(i, occupant) {
//          writeln("Matched");
          id2 |= 0b11;
        } else {
//          writeln("Didn't match");
          id2 |= 0b1;
        }
//       writeIDAsBits();
    }

/*
    writeln("ID for: ");
    print();
    writeln("is:");
    writeIDAsBits();
*/

    return (id, id2);

    proc writeIDAsBits() {
      var mask = 0x1 << 34;
      for i in 1..7 {
        for j in 1..5 {
//          writeln("\nmask is: ", mask);
          write(if id & mask then "1" else "0");
          mask >>= 1;
        }
        write("|");
      }
      writeln();
      mask = 0x1 << 31;
      for i in 1..16 {
        for j in 1..2 {
//          writeln("\nmask is: ", mask);
          write(if id2 & mask then "1" else "0");
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
    else {
      const monsterType = (occupant-1)/slotsPerRoom;
//      writeln("Trying to see what it will take to get ", occupant, " to ", (monsterType*slotsPerRoom)+1);
      return (distance[loc][(monsterType*slotsPerRoom)+1]*(10**((occupant-1)/slotsPerRoom)));
    }
  }

  proc optimisticPathFromRoom(loc) {
    
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
       if newBoard.cost < bestScore && newBoard.optimisticCost() < bestScore {
          const id = newBoard.uniqueID();
          var alreadyDidThis = false;
          if searchedConfigs.contains(id) {
            const prevCost = searchedConfigs[id];
            if prevCost < newBoard.cost {
              alreadyDidThis = true;
/*
              writeln("We've seen this config before:");
              newBoard.print();
*/
            } else {
              searchedConfigs.replace(id, newBoard.cost);
            }
          } else {
            searchedConfigs.add(id, newBoard.cost);

            if debug then
              if searchedConfigs.size%100 == 0 then
                writeln(searchedConfigs.size);

          }

          if !alreadyDidThis then
            newBoard.simulate(depth+1);
/*
        } else {
            if (newBoard.cost >= bestScore && newBoard.cost - bestScore < thresh) {
              writeln("--> pruning a score of ", newBoard.cost, " <--");
              newBoard.print();
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
board.uniqueID();
board.simulate();
writeln("Best score was: ", bestScore);
