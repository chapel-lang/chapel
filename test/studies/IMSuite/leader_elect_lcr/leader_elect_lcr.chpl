 /*
    leader_elect_lcr aims to elect a leader from a set of nodes,
    on the basis of leader election algorithm by Le Lang et al..
    The algorithm is aimed towards unidirectional ring networks.

    @author Suyash Gupta
    @author V Krishna Nandivada

    Ported By: Kushal Singh

    The original program can be viewed here : http://www.cse.iitm.ac.in/~krishna/imsuite/
 */

 /* This file was derived from the IMSuite Benchamark Suite.

    This file is licensed to You under the Eclipse Public License (EPL);
    You may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    http://www.opensource.org/licenses/eclipse-1.0.php

   (C) Copyright IMSuite 2013-present.
 */

 pragma "error mode fatal"
 module leader_elect_lcr {
     use IO;

     config const inputFile = "input/inputleader_elect_lcr_8.txt";
     config const outputFile = "outputleader_elect_lcr_8.txt";

     /* Parameter to enable verification of the output */
     config const verify = false;

     /* Stores the total number of process */
     var processes: int;
     /* Represents the domain over the number of nodes */
     var D : domain(1);
     /* Stores the id's of each of the nodes in the ring */
     var IdStore: [D] int;

     /* Parameters to enable execution with load */
     config const loadValue=0;
     var nval:[D] int;

    /*
        ```Process``` specifies the structure for each abstract node
        part of the Leader election algorithm.
    */
    class Process {
        /* Specifies the identifier for each node. */
        var id: int;
        /* Represents the identifier that the process will send to its neighbor. */
        var send: int;
        /* Represents the identifier of the leader. */
        var leaderId: int;
        /* Represents the identifier that the process receives from its neighbor. */
        var receivedId: int;
        /* Represents the status of the process, set to true for leader. */
        var status: bool;
     }

     /* Abstract node representation */
     var processSet: [D] owned Process?;

    /*
        Acts as the starting point for the program execution.
        ```main``` performs the task of accepting the input from the user
        specified file, electing the leader and transmitting information,
        printing the output and validating the result.
    */
     proc main(){

        var inFile = open(inputFile, iomode.r);
        var reader = inFile.reader();
        processes = reader.read(int);
        D = {0..(processes-1)};

        for i in 0..(processes-1) do IdStore[i]=reader.read(int);

        reader.close();
        inFile.close();

        initialize();
        for round in 0..processes-1 do leader_elect();
        transmitLeader();

        if(verify) then outputVerifier();
        printLeader();

        if(loadValue != 0) {
            var sumval: real = 0;
            for i in 0..(processes-1) do sumval = sumval + nval[i];
            if(sumval > 0) then writeln();
        }
     }


    /* Initializes all the fields of the abstract node. */
    proc initialize() {
        forall (i,p) in zip(D,processSet) {
            const pid = IdStore[i];
            p = new Process(id=pid, leaderId=pid, send=pid,  status=false);
        }
    }

    /*
        Aims to busy the threads by introducing the no-op instructions
        equivalent to the amount of load specified.
        :arg  weight:    Specifies the current load value for a thread.
        :type: int(64)
        :returns: Updated load value.
        :rtype: int(64)
    */
    proc loadweight(weight: int): int {
        var count = 0;
        for i in 0..(loadValue-1) do count+=1;
        return count+weight;
    }

    /* Aims at selecting the leader from a set of nodes. */
    proc leader_elect() {
        forall (i,p) in zip(D,processSet) {
            var x : int = (i + 1) % (processes);
            var sval : int = p!.send;
            sendMessage(x, sval);

            if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
        }

        forall (i,pQ) in zip(D,processSet) {
            const p = pQ!;
            if(p.receivedId > p.leaderId) {
                p.send = p.receivedId;
                p.leaderId = p.receivedId;
            }
            else if(p.receivedId == p.id) {
                p.status = true;
                p.leaderId = p.id;
            }

            if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
        }
    }

    /* Transmits the message from one node to another. */
    proc transmitLeader() {
        var leader : int;
        var k : int;
        forall (i,p) in zip(D,processSet) with (ref k) {
            if p!.status then k=i+1;
        }

        leader = k;
        for i in 0..(processes-1) {
            k = (k+1)%processes;
            setLeader(leader, k);
        }
        processSet[leader-1]!.leaderId = leader;
    }

    /* Sets the leader for a node.
        :arg uNode:    Leader node.
        :type: int(64)
        :arg aNode:    Node whose leader has to be set.
        :type: int(64)
    */
    proc setLeader(uNode: int, aNode: int) {
        processSet[aNode]!.leaderId = uNode;
    }

    /* Sends message to the neighbor.
        :arg receiver: Node which receives the message.
        :type: int(64)
        :arg sval: Message value.
        :type: int(64)
    */
    proc sendMessage(receiver: int, sval: int) {
        processSet[receiver]!.receivedId = sval;
    }

    /* Writes the leader to the user specified file. */
    proc printLeader() {
        var outfile = open(outputFile, iomode.cw);
        var writer = outfile.writer();
        writer.writeln("Leader: ", processSet[0]!.leaderId);
        writeln("Leader: ", processSet[0]!.leaderId);
        writer.close();
        outfile.close();
    }

    /* Validates the output resulting from the execution of the algorithm. */
    proc outputVerifier() {
        var max: int = -1;
        for i in 0..(processes-1) {
            if(max < IdStore[i]) then max = IdStore[i];
        }
        if max == processSet[ (processSet[0]!.leaderId)-1 ]!.id then
          writeln("Output verified");
    }
}
