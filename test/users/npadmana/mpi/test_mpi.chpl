/* A collection of tests/demonstrations of the C-MPI bindings.

These tests are an implementation of the examples here :
  https://computing.llnl.gov/tutorials/mpi/

Each test is in a separate procedure, each of which ends with an
 MPI_Barrier (this is especially important since, in some cases,
 we only use a subset of the ranks).

*/

/* This initializes MPI by default, you can turn
that off with the autoInitMPI parameter
 */
use MPI;
use C_MPI; // Include the C-API, to reduce verbosity of the code.
use Random;

const requiredSize = 4;

proc main() {
  writeln("This is the main Chapel program....");
  coforall loc in Locales do on loc {
    writef("000 : This is Chapel locale %i saying Hello, MPI!\n", here.id);
    MPI_Barrier(MPI_COMM_WORLD);
    spmd();
  }
  writeln("We're back in the main Chapel program now.....");
}

proc spmd() {
  const worldSize = commSize();
  if requiredSize != worldSize {
    writef("Please run with at least %i ranks..\n",requiredSize);
    MPI_Abort(MPI_COMM_WORLD, 10);
  }

  hello();
  point2point();
  ring();
  pi();
  test_scatter();
  test_structure();
  test_allgather();
  test_newcomm();
}


proc hello() {
  /* Simple test of MPI initialization */
  const worldRank = commRank(),
        worldSize = commSize();
  writef("HELLO : %i : This is rank %i of %i processes saying Hello, Chapel!\n",worldRank,
            worldRank, worldSize);
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Simple blocking point-to-point communication */
proc point2point() {

  const worldRank = commRank(),
        worldSize = commSize();
  if worldRank < 2 {
    var recvpi : real = 0.0,
        sendpi : real = 3.1415926;
    var stat : MPI_Status;

    if worldRank==0 {
      MPI_Send(sendpi, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
      MPI_Recv(recvpi, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, stat);
    } else {
      MPI_Recv(recvpi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, stat);
      MPI_Send(sendpi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    var count = stat.getCount(MPI_DOUBLE);

    writef("P2P : Task %i: Received %i pi=%r from task %i with tag %i\n",
        worldRank, count, recvpi, stat.MPI_SOURCE, stat.MPI_TAG);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Non-blocking communication in a ring */
proc ring() {

  var worldRank = commRank(),
      worldSize = commSize();
  var left = mod(worldRank-1, worldSize);
  var right = mod(worldRank+1, worldSize);
  var toleft : c_int = 1;
  var toright : c_int = 2;
  var fromleft = toright,
      fromright = toleft;


  var buf : [1..2]int(32);
  var requests : [1..4]MPI_Request;
  var status : [1..4]MPI_Status;

  MPI_Irecv(buf[1], 1, MPI_INT, left, fromleft, MPI_COMM_WORLD, requests[1]);
  MPI_Irecv(buf[2], 1, MPI_INT, right, fromright, MPI_COMM_WORLD, requests[2]);

  MPI_Isend(worldRank, 1, MPI_INT, left, toleft, MPI_COMM_WORLD, requests[3]);
  MPI_Isend(worldRank, 1, MPI_INT, right, toright, MPI_COMM_WORLD, requests[4]);

  MPI_Waitall(4, requests, status);

  writef("RING : Rank %i recieved %i from the left, and %i from the right\n",worldRank, buf[1], buf[2]);
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Compute pi -- test MPI_Reduce */
proc pi() {
  const worldRank = commRank(),
        worldSize = commSize();
  const N=50000;
  var x : [0.. #N] real;
  var y : [0.. #N] real;

  fillRandom(x, here.id*2 + 1); fillRandom(y, here.id*4 + 1);
  var sum = 0.0;
  forall (x1,y1) in zip(x,y) with (+ reduce sum) {
    if (x1*x1+y1*y1) <= 1 then sum += 1;
  }
  sum /= N;

  var summ : real;
  MPI_Reduce(sum, summ, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if worldRank==0 {
    summ *= 4/worldSize;
    writef("PI : I estimate pi to be %5.2r\n",summ);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Test scatters. */
proc test_scatter() {
  const worldRank = commRank(),
        worldSize = commSize();
  var arr : [{0..3,0..3}]real(32);
  forall (i,j) in arr.domain {
    arr[i,j] = (i*4 + j):real(32);
  }
  var recbuf : [0..3]real(32);

  // Use MPI Scatter
  {
    MPI_Scatter(arr[0,0], 4, MPI_FLOAT, recbuf[0], 4, MPI_FLOAT, 0, MPI_COMM_WORLD);
    writef("SCATTER1 : Rank %i :%t\n",worldRank,recbuf);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  // Use a contiguous data type
  {
    arr += 1:real(32);
    var rowtype : MPI_Datatype;
    var stat : MPI_Status;

    MPI_Type_contiguous(4, MPI_FLOAT, rowtype);
    MPI_Type_commit(rowtype);

    if worldRank==0 {
      for irank in 0.. #worldSize {
        if irank == 0 {
          recbuf = arr[0,..];
        } else {
          MPI_Send(arr[irank,0], 1, rowtype, irank, 1, MPI_COMM_WORLD);
        }
      }
    } else {
      MPI_Recv(recbuf[0], 4, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, stat);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    writef("SCATTER2 : Rank %i :%t\n",worldRank,recbuf);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Type_free(rowtype);
  }

  // Use a vector data dtype - send columns
  {
    var coltype : MPI_Datatype;
    var stat : MPI_Status;

    MPI_Type_vector(4, 1, 4, MPI_FLOAT, coltype);
    MPI_Type_commit(coltype);

    if worldRank==0 {
      for irank in 0.. #worldSize {
        if irank == 0 {
          recbuf = arr[..,0];
        } else {
          MPI_Send(arr[0, irank], 1, coltype, irank, 1, MPI_COMM_WORLD);
        }
      }
    } else {
      MPI_Recv(recbuf[0], 4, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, stat);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    writef("SCATTER3 : Rank %i :%t\n",worldRank,recbuf);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Type_free(coltype);
  }

  // Use an indexed data type
  {
    var arr2 = reshape(arr, {0.. #16});
    var indextype : MPI_Datatype;
    var stat : MPI_Status;

    var blocklength : [0.. #2]c_int = [2:c_int,2:c_int];
    var displ : [0.. #2]c_int = [5:c_int,12:c_int];

    MPI_Type_indexed(2, blocklength, displ, MPI_FLOAT, indextype);
    MPI_Type_commit(indextype);

    if worldRank==0 {
      for irank in 0.. #worldSize {
        if irank == 0 {
          recbuf[0.. #2] = arr2[5.. #2];
          recbuf[2.. #2] = arr2[12.. #2];
        } else {
          MPI_Send(arr2[0], 1, indextype, irank, 1, MPI_COMM_WORLD);
        }
      }
    } else {
      MPI_Recv(recbuf[0], 4, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, stat);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    writef("SCATTER4 : Rank %i :%t\n",worldRank, recbuf);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Type_free(indextype);
  }

}


/* Test structure derived type.

Note that this particular function is deprecated in MPI-1.1
*/
proc test_structure() {
  const worldRank = commRank(),
        worldSize = commSize();
  record Particle {
    var x, y, z, vel : real;
    var n, typ : int;
  }

  var ptype : MPI_Datatype,
      offsets : [0..1]MPI_Aint,
      extent: MPI_Aint,
      block : [0..1]c_int = [4:c_int, 2:c_int],
      oldtype : [0..1]MPI_Datatype = [MPI_DOUBLE, MPI_LONG];

  offsets[0] = 0;
  MPI_Type_extent(MPI_DOUBLE, extent);
  offsets[1] = 4*extent;
  MPI_Type_struct(2, block, offsets, oldtype, ptype);
  MPI_Type_commit(ptype);


  var a : [0.. #10] Particle;
  if worldRank==0 {
    for (i1, p1) in zip(0.. , a) {
      p1.x = i1;
      p1.y = -i1;
      p1.z = i1;
      p1.vel = 0.25;
      p1.n = i1;
      p1.typ = i1 % 2;
    }
  }

  MPI_Bcast(a[0], 10, ptype, 0, MPI_COMM_WORLD);

  writeln("TYPE : Rank : ", worldRank, " : ", a[3]);

  MPI_Type_free(ptype);
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Test Allgather

*/
proc test_allgather() {
  var worldRank = commRank(),
      worldSize = commSize();
  var ranks : [0.. #worldSize]c_int;

  MPI_Allgather(worldRank, 1, MPI_INT, ranks[0], 1, MPI_INT, MPI_COMM_WORLD);
  writeln("ALLGATHER : Rank ", worldRank, " : ", ranks);
  MPI_Barrier(MPI_COMM_WORLD);
}

/* MPI make communicator */
proc test_newcomm() {
  var worldRank = commRank(),
      worldSize = commSize();
  var comm : MPI_Comm,
      ranks1 : [0..1]c_int = [0:c_int, 1:c_int],
      ranks2 : [0..1]c_int = [2:c_int, 3:c_int],
      sum : c_int,
      newrank : c_int,
      origgrp, newgrp : MPI_Group;

  MPI_Comm_group(MPI_COMM_WORLD, origgrp);
  if worldRank < 2 {
    MPI_Group_incl(origgrp, 2, ranks1[0], newgrp);
  } else {
    MPI_Group_incl(origgrp, 2, ranks2[0], newgrp);
  }
  MPI_Comm_create(MPI_COMM_WORLD, newgrp, comm);
  MPI_Allreduce(worldRank, sum, 1, MPI_INT, MPI_SUM, comm);

  MPI_Comm_rank(comm, newrank);

  writef("ALLGATHER : Rank = %i, new rank = %i, sum = %i\n",worldRank, newrank, sum);
  MPI_Barrier(MPI_COMM_WORLD);
}
