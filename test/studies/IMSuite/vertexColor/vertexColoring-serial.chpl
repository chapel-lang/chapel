 /*
    Vertex-Coloring aims to color the vertices of a tree with three colors.

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
module vertexColoring {

  use Random, IO;
    use VisualDebug;

    config const inputFile = "input/inputvertexColoring_64_-rn.txt";
    config const outputFile = "outputvertexColoring_64-rn.txt";

    /* Parameter to enable verification of the output */
    config const verify = false;

    /* Stores the total number of nodes in the graph */
    var nodes: int;
    var colorLabel: int;
    /* Represents the domain over the number of nodes */
    var D : domain(1);
    /* Represents the domain of the adjacency matrix */
    var N : domain(2);
    /* Represents the adjacency matrix to store the edges in the graph */
    var adj_graph :[N] int;
    /* Stores the parent node corresponding to every node */
    var parent:[D] int;
    /* Represents the root of the graph */
    var root: int;
    var nlabel:[D] int;

    /* Parameters to enable execution with load */
    config var loadValue=0;
    var nval:[D] int;

    /*
        ```Node``` specifies the structure for each abstract node
        part of the Vertex Coloring Algorithm.
    */
    class Node {
      /* Specifies the parent of a node */
      var parent: int;
      /* Specifies the color of a node */
      var color: int;
      /* Specifies the color received by a node */
      var receivedColor: int;
      /* Specifies the domain for the children array */
      var neighbourD : domain(1);
      /* List of neighbor ids */
      var children: [neighbourD] int;
    }

    /** Abstract node representation */
    var nodeSet:[D] owned Node?;
    var again:[D] bool;

    /*
        Acts as the starting point for the program execution.
        ```main``` performs the task of accepting the input from the user
        specified file, electing the leader and transmitting information,
        printing the output and validating the result.
    */
    proc main() {

        var inFile = open(inputFile, iomode.r);
        var reader = inFile.reader();

        nodes = reader.read(int);
        D = {0..(nodes-1)};
        N = {0..(nodes-1), 0..(nodes-1)};
        root = reader.read(int);
        var line:string;

        for i in D {
            reader.read(line);
            for j in 0..(line.size-1) do adj_graph(i,j)=(line[j+1]:int);
        }

        for i in D do parent[i]=reader.read(int);

        reader.close();
        inFile.close();

        initialize();
        run();
        six2three();

        if(verify) then outputVerifier();

        printOutput();

        if(loadValue != 0) {
            var sumval: real = 0;
            for i in D do sumval = sumval + nval[i];
            if(sumval > 0) then writeln();
        }
    }

    /* Initializes all the fields of the abstract node. */
    proc initialize() {
        forall (nodeElem, num_label, i) in zip(nodeSet, nlabel, D) {
            num_label = i;
            nodeElem = new Node();
            const node = nodeElem!;
            node.parent = parent[i];
            var count = 0;
            for j in D {
                if(adj_graph(i,j) == 1 && node.parent != j) then count+=1;
            }

            node.neighbourD = {1..count};
            count=1;

            for j in D {
                if(adj_graph(i,j) == 1 && node.parent != j) {
                    node.children[count] = j;
                    count+=1;
                }
            }
            node.color = num_label;
        }

        colorLabel = log2(nodes);
        if(1<<colorLabel < nodes) then colorLabel+=1;
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
        var count=0;
        for i in 0..loadValue-1 do count+=1;
        return count+weight;
    }

    /* Runs the algorithm till the graph consists of atmost six colors. */
    proc run() {
        nodeSet[root]!.color = 0;
        do {
            sixColor();
        }while(checkAgain());
    }

    /* Determines the number of different colors used in the graph.
       :returns:    true if there is no color >= 6.
       :rtype: bool
    */
    proc checkAgain(): bool {
        var flag = || reduce again;
        return flag;
    }

    /* Reduces the number of colors used in the graph to six. */
    proc sixColor(){
        forall (i, nodeQ) in zip(D, nodeSet) {
            const node = nodeQ!;
            forall j in 1..node.children.size {
                sendColor(node.children[j], node.color);
            }

            if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
        }

        forall (i, nodeQ) in zip(D, nodeSet) {
            const node = nodeQ!;
            if(i!=root){
                again[i]=false;
                var xored = node.receivedColor ^ node.color;
                for k in 0..colorLabel-1 {
                    var pval = 1<<k;
                    var nand :int = xored & pval;
                    if(nand == pval) {
                        var nxored = node.color & pval;
                        if(nxored == 0) then
                            node.color = 2*k + 0;
                        else
                            node.color = 2*k + 1;
                        break;
                    }
                }

                if(node.color >= 6) then again[i] = true;
            }

            if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
        }
    }


   /* Reduces the number of colors from six to three. */
   proc six2three() {
       for k in 1..3 {
            var x = 6-k;
            var randStream = new owned RandomStream(real, 3);
            var ncolor : int = randStream.getNext(): int;
            ncolor = ncolor%3;
            shiftDown();
            if(nodeSet[root]!.color == ncolor) then ncolor = (ncolor+1)%3;
            nodeSet[root]!.color = ncolor;
            
            forall (i, nodeQ) in zip(D, nodeSet) {
                const node = nodeQ!;
                var cparent=0,cchild=0;
                if(node.color == x) {
                    cparent=getColor(node.parent);
                    if(node.children.size >0) then
                        cchild=getColor(node.children[1]);
                    if(cparent+cchild == 1) then
                        node.color=2;
                    else if(cparent+cchild == 2) then
                        node.color=1;
                    else if( (cparent+cchild >=3) && (cparent+cchild<=5)) {
                        if(cparent != 0 && cchild != 0) then
                            node.color=0;
                        else
                            node.color=1;
                    } else
                        node.color = 0;
                }

                if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
            }
        }

    }

   /* Shifts the color of parent down to its children. */
   proc shiftDown() {
        forall (i, nodeQ) in zip(D, nodeSet) {
            const node = nodeQ!;
            for j in 1..node.children.size {
                    sendColor(node.children[j], node.color);
            if(loadValue != 0) then nval[i] = loadweight(nval[i]+i);
            }
        }

        forall (i, nodeQ) in zip(D, nodeSet) {
            const node = nodeQ!;
            if(i != root) then node.color = node.receivedColor;
            if(loadValue!=0) then nval[i] = loadweight(nval[i]+i);
        }
    }

    /* Provides the color of the ```aNode```.
       :arg aNode: node whose color value is required.
       :type: int(64)
       :returns: color of ```aNode```.
       :rtype: int(64);
    */
    proc getColor(aNode:int):int {
        return nodeSet[aNode]!.color;
    }

    /* Sends the color of the parent node to a child node.
       :arg childNode: node whose color is to be changed
    */
    proc sendColor(childNode: int, acolor:int) {
        nodeSet[childNode]!.receivedColor = acolor;
    }

    proc printOutput() {
        var outfile = open(outputFile, iomode.cw);
        var writer = outfile.writer();
        for (q, node) in zip(D, nodeSet!) {
            writer.writeln("Node ", q, ": \t Color ", node.color);
            writeln("Node ", q, ": \t Color ", node.color);
        }
        writer.close();
        outfile.close();
    }

    /* Validates the output resulting from the execution of the algorithm. */
    proc outputVerifier() {
        var count =0;
        var colormat : [D] int = -1;
        var flag = false;

        for (i, nodeQ) in zip(D, nodeSet) {
            const node = nodeQ!;
            if(colormat[node.color] <0) {
                colormat[node.color]=0;
                count+=1;
            }
        }
        if(count <= 3) {
            flag = false;
            for (i, nodeQ) in zip(D, nodeSet) {
                const node = nodeQ!;
                if(i != root){
                    if(node.color == nodeSet[node.parent]!.color) {
                        flag = true;
                        break;
                    }
                }

                for j in 1..node.children.size {
                    if(node.color == nodeSet[node.children[j]]!.color) {
                        flag = true;
                        break;
                    }
                }

            }
            if(!flag) then writeln("Output verified");
        }
    }
}
