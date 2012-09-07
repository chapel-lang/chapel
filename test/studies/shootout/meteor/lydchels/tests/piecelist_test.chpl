module piecelist_test {
  use Meteor;

  // Things to test:
  var list: PieceList = new PieceList();
  var cell1: PieceCell = new PieceCell();
  var cell2: PieceCell = new PieceCell();
  var cell3: PieceCell = new PieceCell();
  var cell4: PieceCell = new PieceCell();
  var cell5: PieceCell = new PieceCell();



  //  Clear Box
  //    initialization
  //      empty to start
  proc empty() {
    if list.currentToAdd != 1 || list.pieces[1] != nil || 
      list.pieces[2] != nil || list.pieces[3] != nil || 
      list.pieces[4] != nil || list.pieces[5] != nil {
      writeln("Fails: PieceList should initialize to an empty PieceList");
      writeln(list);
    } else {
      writeln("Passes: PieceList initialized as empty");
    }
  }
  //    build
  //      first
  proc bFirst() {
    list.build(cell1);
    if list.currentToAdd != 2 {
      writeln("Fails: currentToAdd should have updated");
    } else {
      writeln("Passes: currentToAdd was updated");
    }
    if list.pieces[1] != cell1 {
      writeln("Fails: cell1 should have been added at the front of the list");
    } else {
      writeln("Passes: cell1 was added at the front of the list");
    }
  }
  //      in the middle
  //      new Cell has neighbor Cell in correct position
  //      also, neighbor Cell has new Cell in correct neighbor position  
  proc bMiddleLine() {
    list.build(cell2, 1, EAST);
    if list.pieces[2] == cell2 {
      writeln("Passes: cell2 was correctly added to the list");
      if list.pieces[2].getNeighbor(WEST) != cell1 {
	writeln("Fails: cell2 should have a neighbor to the west");
      } else {
	writeln("Passes: cell2's neighbor was correctly denoted");
      }
      if list.pieces[1].getNeighbor(EAST) != cell2 {
	writeln("Fails: cell1 should now have a neighbor to the east");
      } else {
	writeln("Passes: cell1's new neighbor was correctly denoted");
      }
    } else {
      writeln("Fails: cell2 should have been added to the list");
    }
  }
  //      at the end - want to check currentToAdd here again
  proc bEndLine() {
    list.build(cell3, 2, EAST);
    list.build(cell4, 3, EAST);
    list.build(cell5, 4, EAST);
    if list.currentToAdd != 6 {
      writeln("Fails: currentToAdd should be more than the number of cells");
    } else {
      writeln("Passes: currentToAdd reflects accurately");
    }
    if list.pieces[5] == nil {
      writeln("Fails: cell5 should have been added at the end of the list");
    } else {
      writeln("Passes: cell5 was added to the end of the list");
    }
  }
  //      position specified is already taken
  proc bTaken() {
    list.pieces[3] = nil;
    list.pieces[4] = nil;
    list.pieces[5] = nil;
    list.currentToAdd = 3;
    if list.build(cell3, 2, WEST) {
      writeln("Fails: expected method to return false when position already" +
	      " filled");
    } else {
      if list.pieces[3] != nil {
	writeln("Fails: should not have added cell3 to occupied space");
      } else {
	writeln("Passes: did not add cell3 to occupied space");
	if list.currentToAdd != 3 {
	  writeln("Fails: currentToAdd shouldn't update when space not filled");
	} else {
	  writeln("Passes: currentToAdd reflects correct state");
	}
      }
    }
  }
  //      cell to attach to doesn't exist yet
  proc bUnattached() {
    if list.build(cell3, 4, WEST) {
      writeln("Fails: expected method to return false when cell to attach" +
	      " to doesn't exist");
    } else {
      if list.pieces[3] != nil || list.pieces[5] != nil {
	writeln("Fails: should not have added cell3 to the list");
      } else {
	writeln("Passes: did not add cell3 when neighbor didn't exist");
      }
    }
  }
  //      when new Cell is placed, if that placement would put it next to any
  //                 other Cells, that is reflected in those Cells and the new
  //                 Cell
  proc bAllUpdated() {
    if !list.build(cell3, 2, SOUTHWEST) {
      writeln("Fails: expected method to add cell3 and return true");
    } else {
      if list.pieces[3] != cell3 {
	writeln("Fails: expected cell3 to have been added to the list");
      } else {
	if list.pieces[3].getNeighbor(NORTHWEST) != list.pieces[1] {
	  writeln("Fails: expected cell3 to have multiple neighbors");
	} else if list.pieces[1].getNeighbor(SOUTHEAST) != list.pieces[3] {
	  writeln("Fails: expected cell1 to have updated its neighbors");
	} else {
	  writeln("Passes: all neighbor fields have been correctly set");
	}
      }
    }    
  }
  
  // Cleans up the garbage
  proc cleanUp() {
    delete cell1;
    delete cell2;
    delete cell3;
    delete cell4;
    delete cell5;
    delete list;
  }
}