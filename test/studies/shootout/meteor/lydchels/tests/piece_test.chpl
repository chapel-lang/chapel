module piece_test {
  use Meteor;

  // Things to test:
  var s: PieceList = new PieceList(); // straight line
  var c: PieceList = new PieceList(); // almost forms a circle
  var j: PieceList = new PieceList(); // jagged line
  var b: PieceList = new PieceList(); // interconnected
  var m: PieceList = new PieceList(); // looks like a hammer
  var h: PieceList = new PieceList(); // looks like a rocking horse 

  // Wanted five individual cells for each test piece, so this, while ugly,
  // ought to fulfill that purpose with minimal confusion.
  var s1: PieceCell = new PieceCell(); 
  var s2: PieceCell = new PieceCell();
  var s3: PieceCell = new PieceCell();
  var s4: PieceCell = new PieceCell();
  var s5: PieceCell = new PieceCell();
  var c1: PieceCell = new PieceCell();
  var c2: PieceCell = new PieceCell();
  var c3: PieceCell = new PieceCell();
  var c4: PieceCell = new PieceCell();
  var c5: PieceCell = new PieceCell();
  var j1: PieceCell = new PieceCell();
  var j2: PieceCell = new PieceCell();
  var j3: PieceCell = new PieceCell();
  var j4: PieceCell = new PieceCell();
  var j5: PieceCell = new PieceCell();
  var b1: PieceCell = new PieceCell();
  var b2: PieceCell = new PieceCell();
  var b3: PieceCell = new PieceCell();
  var b4: PieceCell = new PieceCell();
  var b5: PieceCell = new PieceCell();
  var m1: PieceCell = new PieceCell();
  var m2: PieceCell = new PieceCell();
  var m3: PieceCell = new PieceCell();
  var m4: PieceCell = new PieceCell();
  var m5: PieceCell = new PieceCell();
  var h1: PieceCell = new PieceCell();
  var h2: PieceCell = new PieceCell();
  var h3: PieceCell = new PieceCell();
  var h4: PieceCell = new PieceCell();
  var h5: PieceCell = new PieceCell();

  var straight: Piece;
  var circular: Piece;
  var jagged: Piece;
  var block: Piece;
  var mjolnir: Piece;
  var horse: Piece;

  //  Black Box
  //    initialization
  //      if the PieceList provided is not full, returns nil(?)
  proc initialize() {
    s.build(s1);
    s.build(s2, 1, EAST);
    s.build(s3, 2, EAST);
    s.build(s4, 3, EAST);
    s.build(s5, 4, EAST);
    straight = new Piece(s);
    // straight is now finished
    c.build(c1);
    c.build(c2, 1, SOUTHWEST);
    c.build(c3, 2, SOUTHEAST);
    c.build(c4, 3, EAST);
    c.build(c5, 4, NORTHEAST);
    circular = new Piece(c);
    // circular is now finished
    j.build(j1);
    j.build(j2, 1, EAST);
    j.build(j3, 2, NORTHEAST);
    j.build(j4, 3, EAST);
    j.build(j5, 4, NORTHEAST);
    jagged = new Piece(j);
    // jagged is now finished
    b.build(b1);
    b.build(b2, 1, SOUTHEAST);
    b.build(b3, 1, EAST);
    b.build(b4, 3, SOUTHEAST);
    b.build(b5, 3, EAST);
    block = new Piece(b);
    // block is now finished
    m.build(m1);
    m.build(m2, 1, NORTHEAST);
    m.build(m3, 1, SOUTHEAST);
    m.build(m4, 1, EAST);
    m.build(m5, 4, EAST);
    mjolnir = new Piece(m);
    // mjolnir is now finished
    h.build(h1);
    h.build(h2, 1, SOUTHEAST);
    h.build(h3, 2, SOUTHWEST);
    h.build(h4, 2, EAST);
    var fail = new Piece(h);
    if fail != nil {
      writeln("Fails: Only fulls PieceLists can be used to create Pieces");
    } else {
      writeln("Passes: non-full PieceList caused the creation of a nil Piece");
    }
    h.build(h5, 4, SOUTHEAST);
    horse = new Piece(h);
    // horse is now finished
  }

  //  Clear Box
  //    initialization
  //      All Cells are initially unprocessed
  proc iUnprocessed () {
    writeln("Checking processed state of Cells.  No output = Passes");
    forall i in straight.pieceCells {
      if i.isProcessed() {
	writeln("Fails: cells should initially be unprocessed");
      }
    }
  }
  //      initially currentPermutation is 0
  proc iPerm0 () {
    if circular.currentPermutation != 0 {
      writeln("Fails: currentPermutation should initially be set to 0");
    } else {
      writeln("Passes: currentPermutation was initially correct");
    }
  }
  //    rotatePiece
  //      All Cells are rotated
  proc roPi(piece) {
    var t1: PieceCell = new PieceCell(); // t is for test
    var t2: PieceCell = new PieceCell();
    var t3: PieceCell = new PieceCell();
    var t4: PieceCell = new PieceCell();
    var t5: PieceCell = new PieceCell();
    forall i in EAST..NORTHEAST {
      t1.setNeighbor(i, piece.pieceCells[1].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t2.setNeighbor(i, piece.pieceCells[2].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t3.setNeighbor(i, piece.pieceCells[3].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t4.setNeighbor(i, piece.pieceCells[4].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t5.setNeighbor(i, piece.pieceCells[5].getNeighbor(i));
    }
    piece.rotatePiece();
    if (piece.pieceCells[1].getNeighbor(EAST) != t1.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(NORTHEAST) != 
       t1.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[1].getNeighbor(NORTHWEST) != t1.getNeighbor(WEST)) ||
      (piece.pieceCells[1].getNeighbor(WEST) != t1.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHWEST) != 
       t1.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHEAST) != t1.getNeighbor(EAST)) {
      writeln("Fails: cell1 was not rotated properly");
    } else {
      writeln("Passes: cell1 was rotated properly");
    }
    if (piece.pieceCells[2].getNeighbor(EAST) != t2.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(NORTHEAST) != 
       t2.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[2].getNeighbor(NORTHWEST) != t2.getNeighbor(WEST)) ||
      (piece.pieceCells[2].getNeighbor(WEST) != t2.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHWEST) != 
       t2.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHEAST) != t2.getNeighbor(EAST)) {
      writeln("Fails: cell2 was not rotated properly");
    } else {
      writeln("Passes: cell2 was rotated properly");
    }
    if (piece.pieceCells[3].getNeighbor(EAST) != t3.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(NORTHEAST) != 
       t3.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[3].getNeighbor(NORTHWEST) != t3.getNeighbor(WEST)) ||
      (piece.pieceCells[3].getNeighbor(WEST) != t3.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHWEST) != 
       t3.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHEAST) != t3.getNeighbor(EAST)) {
      writeln("Fails: cell3 was not rotated properly");
    } else {
      writeln("Passes: cell3 was rotated properly");
    }
    if (piece.pieceCells[4].getNeighbor(EAST) != t4.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(NORTHEAST) != 
       t4.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[4].getNeighbor(NORTHWEST) != t4.getNeighbor(WEST)) ||
      (piece.pieceCells[4].getNeighbor(WEST) != t4.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHWEST) != 
       t4.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHEAST) != t4.getNeighbor(EAST)) {
      writeln("Fails: cell4 was not rotated properly");
    } else {
      writeln("Passes: cell4 was rotated properly");
    }
    if (piece.pieceCells[5].getNeighbor(EAST) != t5.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(NORTHEAST) != 
       t5.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[5].getNeighbor(NORTHWEST) != t5.getNeighbor(WEST)) ||
      (piece.pieceCells[5].getNeighbor(WEST) != t5.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHWEST) != 
       t5.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHEAST) != t5.getNeighbor(EAST)) {
      writeln("Fails: cell5 was not rotated properly");
    } else {
      writeln("Passes: cell5 was rotated properly");
    }
  }
  //    flipPiece
  //      All Cells are flipped
  proc fP(piece) {
    var t1: PieceCell = new PieceCell(); // t is for test
    var t2: PieceCell = new PieceCell();
    var t3: PieceCell = new PieceCell();
    var t4: PieceCell = new PieceCell();
    var t5: PieceCell = new PieceCell();
    forall i in EAST..NORTHEAST {
      t1.setNeighbor(i, piece.pieceCells[1].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t2.setNeighbor(i, piece.pieceCells[2].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t3.setNeighbor(i, piece.pieceCells[3].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t4.setNeighbor(i, piece.pieceCells[4].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t5.setNeighbor(i, piece.pieceCells[5].getNeighbor(i));
    }
    piece.flipPiece();
    if (piece.pieceCells[1].getNeighbor(EAST) != t1.getNeighbor(WEST)) ||
      (piece.pieceCells[1].getNeighbor(NORTHEAST) != 
       t1.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[1].getNeighbor(NORTHWEST) != t1.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(WEST) != t1.getNeighbor(EAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHWEST) != 
       t1.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHEAST) != t1.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell1 was not flipped properly");
    } else {
      writeln("Passes: cell1 was flipped properly");
    }
    if (piece.pieceCells[2].getNeighbor(EAST) != t2.getNeighbor(WEST)) ||
      (piece.pieceCells[2].getNeighbor(NORTHEAST) != 
       t2.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[2].getNeighbor(NORTHWEST) != t2.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(WEST) != t2.getNeighbor(EAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHWEST) != 
       t2.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHEAST) != t2.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell2 was not flipped properly");
    } else {
      writeln("Passes: cell2 was flipped properly");
    }
    if (piece.pieceCells[3].getNeighbor(EAST) != t3.getNeighbor(WEST)) ||
      (piece.pieceCells[3].getNeighbor(NORTHEAST) != 
       t3.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[3].getNeighbor(NORTHWEST) != t3.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(WEST) != t3.getNeighbor(EAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHWEST) != 
       t3.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHEAST) != t3.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell3 was not flipped properly");
    } else {
      writeln("Passes: cell3 was flipped properly");
    }
    if (piece.pieceCells[4].getNeighbor(EAST) != t4.getNeighbor(WEST)) ||
      (piece.pieceCells[4].getNeighbor(NORTHEAST) != 
       t4.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[4].getNeighbor(NORTHWEST) != t4.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(WEST) != t4.getNeighbor(EAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHWEST) != 
       t4.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHEAST) != t4.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell4 was not flipped properly");
    } else {
      writeln("Passes: cell4 was flipped properly");
    }
    if (piece.pieceCells[5].getNeighbor(EAST) != t5.getNeighbor(WEST)) ||
      (piece.pieceCells[5].getNeighbor(NORTHEAST) != 
       t5.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[5].getNeighbor(NORTHWEST) != t5.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(WEST) != t5.getNeighbor(EAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHWEST) != 
       t5.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHEAST) != t5.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell5 was not flipped properly");
    } else {
      writeln("Passes: cell5 was flipped properly");
    }
  }
  //    nextPermutation
  //      if current permutation is last rotation before flip
  proc nPFlipNext (piece) {
    var t1: PieceCell = new PieceCell(); // t is for test
    var t2: PieceCell = new PieceCell();
    var t3: PieceCell = new PieceCell();
    var t4: PieceCell = new PieceCell();
    var t5: PieceCell = new PieceCell();
    forall i in EAST..NORTHEAST {
      t1.setNeighbor(i, piece.pieceCells[1].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t2.setNeighbor(i, piece.pieceCells[2].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t3.setNeighbor(i, piece.pieceCells[3].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t4.setNeighbor(i, piece.pieceCells[4].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t5.setNeighbor(i, piece.pieceCells[5].getNeighbor(i));
    }
    piece.currentPermutation = 5;
    piece.nextPermutation();
    if (piece.pieceCells[1].getNeighbor(EAST) != t1.getNeighbor(WEST)) ||
      (piece.pieceCells[1].getNeighbor(NORTHEAST) != 
       t1.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[1].getNeighbor(NORTHWEST) != t1.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(WEST) != t1.getNeighbor(EAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHWEST) != 
       t1.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHEAST) != t1.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell1 should have been flipped");
    } else {
      writeln("Passes: cell1 was flipped");
    }
    if (piece.pieceCells[2].getNeighbor(EAST) != t2.getNeighbor(WEST)) ||
      (piece.pieceCells[2].getNeighbor(NORTHEAST) != 
       t2.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[2].getNeighbor(NORTHWEST) != t2.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(WEST) != t2.getNeighbor(EAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHWEST) != 
       t2.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHEAST) != t2.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell2 should have been flipped");
    } else {
      writeln("Passes: cell2 was flipped");
    }
    if (piece.pieceCells[3].getNeighbor(EAST) != t3.getNeighbor(WEST)) ||
      (piece.pieceCells[3].getNeighbor(NORTHEAST) != 
       t3.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[3].getNeighbor(NORTHWEST) != t3.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(WEST) != t3.getNeighbor(EAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHWEST) != 
       t3.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHEAST) != t3.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell3 should have been flipped");
    } else {
      writeln("Passes: cell3 was flipped");
    }
    if (piece.pieceCells[4].getNeighbor(EAST) != t4.getNeighbor(WEST)) ||
      (piece.pieceCells[4].getNeighbor(NORTHEAST) != 
       t4.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[4].getNeighbor(NORTHWEST) != t4.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(WEST) != t4.getNeighbor(EAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHWEST) != 
       t4.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHEAST) != t4.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell4 should have been flipped");
    } else {
      writeln("Passes: cell4 was flipped");
    }
    if (piece.pieceCells[5].getNeighbor(EAST) != t5.getNeighbor(WEST)) ||
      (piece.pieceCells[5].getNeighbor(NORTHEAST) != 
       t5.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[5].getNeighbor(NORTHWEST) != t5.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(WEST) != t5.getNeighbor(EAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHWEST) != 
       t5.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHEAST) != t5.getNeighbor(SOUTHWEST)) {
      writeln("Fails: cell5 should have been flipped");
    } else {
      writeln("Passes: cell5 was flipped");
    }
  }
  //      if permutation is normal
  proc nPNormal (piece) {
    var t1: PieceCell = new PieceCell(); // t is for test
    var t2: PieceCell = new PieceCell();
    var t3: PieceCell = new PieceCell();
    var t4: PieceCell = new PieceCell();
    var t5: PieceCell = new PieceCell();
    forall i in EAST..NORTHEAST {
      t1.setNeighbor(i, piece.pieceCells[1].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t2.setNeighbor(i, piece.pieceCells[2].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t3.setNeighbor(i, piece.pieceCells[3].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t4.setNeighbor(i, piece.pieceCells[4].getNeighbor(i));
    }
    forall i in EAST..NORTHEAST {
      t5.setNeighbor(i, piece.pieceCells[5].getNeighbor(i));
    }
    piece.nextPermutation();
    if (piece.pieceCells[1].getNeighbor(EAST) != t1.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(NORTHEAST) != 
       t1.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[1].getNeighbor(NORTHWEST) != t1.getNeighbor(WEST)) ||
      (piece.pieceCells[1].getNeighbor(WEST) != t1.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHWEST) != 
       t1.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[1].getNeighbor(SOUTHEAST) != t1.getNeighbor(EAST)) {
      writeln("Fails: cell1 should have been rotated");
    } else {
      writeln("Passes: cell1 was rotated");
    }
    if (piece.pieceCells[2].getNeighbor(EAST) != t2.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(NORTHEAST) != 
       t2.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[2].getNeighbor(NORTHWEST) != t2.getNeighbor(WEST)) ||
      (piece.pieceCells[2].getNeighbor(WEST) != t2.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHWEST) != 
       t2.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[2].getNeighbor(SOUTHEAST) != t2.getNeighbor(EAST)) {
      writeln("Fails: cell2 should have been rotated");
    } else {
      writeln("Passes: cell2 was rotated");
    }
    if (piece.pieceCells[3].getNeighbor(EAST) != t3.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(NORTHEAST) != 
       t3.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[3].getNeighbor(NORTHWEST) != t3.getNeighbor(WEST)) ||
      (piece.pieceCells[3].getNeighbor(WEST) != t3.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHWEST) != 
       t3.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[3].getNeighbor(SOUTHEAST) != t3.getNeighbor(EAST)) {
      writeln("Fails: cell3 should have been rotated");
    } else {
      writeln("Passes: cell3 was rotated");
    }
    if (piece.pieceCells[4].getNeighbor(EAST) != t4.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(NORTHEAST) != 
       t4.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[4].getNeighbor(NORTHWEST) != t4.getNeighbor(WEST)) ||
      (piece.pieceCells[4].getNeighbor(WEST) != t4.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHWEST) != 
       t4.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[4].getNeighbor(SOUTHEAST) != t4.getNeighbor(EAST)) {
      writeln("Fails: cell4 should have been rotated");
    } else {
      writeln("Passes: cell4 was rotated");
    }
    if (piece.pieceCells[5].getNeighbor(EAST) != t5.getNeighbor(NORTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(NORTHEAST) != 
       t5.getNeighbor(NORTHWEST)) || 
      (piece.pieceCells[5].getNeighbor(NORTHWEST) != t5.getNeighbor(WEST)) ||
      (piece.pieceCells[5].getNeighbor(WEST) != t5.getNeighbor(SOUTHWEST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHWEST) != 
       t5.getNeighbor(SOUTHEAST)) ||
      (piece.pieceCells[5].getNeighbor(SOUTHEAST) != t5.getNeighbor(EAST)) {
      writeln("Fails: cell5 should have been rotated");
    } else {
      writeln("Passes: cell5 was rotated");
    }
  }
  //      if permutation is on flipped side
  proc nPNormalFlipped () {

  }
  //    resetProcessed
  //      All Cells are no longer processed
  proc rePr () {

  }

  proc cleanUp() {
    delete straight;
    delete circular;
    delete jagged;
    delete block;
    delete mjolnir;
    delete horse;
    delete s;
    delete c;
    delete j;
    delete b;
    delete m;
    delete h;
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
    delete j1;
    delete j2;
    delete j3;
    delete j4;
    delete j5;
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete b5;
    delete m1;
    delete m2;
    delete m3;
    delete m4;
    delete m5;
    delete h1;
    delete h2;
    delete h3;
    delete h4;
    delete h5;
  }
}