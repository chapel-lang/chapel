 /* This file is part of IMSuite Benchamark Suite.

    This file is licensed to You under the Eclipse Public License (EPL);
    You may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    http://www.opensource.org/licenses/eclipse-1.0.php

   (C) Copyright IMSuite 2013-present.
 */

 /*
    vertexColoring aims to color the vertices of a tree with
    three colors.

    @author Suyash Gupta
    @author V Krishna Nandivada

    Ported By: Kushal Singh
 */

module vertexColoring {

    use Random;
    use VisualDebug;

    config var inputFile = "inputvertexColoring_16_max.txt";
    config var outputFile = "outputvertexColoring.txt";
    config var verify = false;

    var nodes: int;
    var colorLabel: int;
    var D : domain(1);
    var adj_graph :[D][D] int;
    var parent:[D] int;
    var root: int;
    var nlabel:[D] int;
    var colormat:[D] int;

    /* Parameters to enable execution with load */
    config var loadValue=0;
    var nval:[D] int;

    class Node {
      var parent: int;                  // specifies the parent of a node
      var color: int;                   // specifies the color of a node
      var receivedColor: int;           // specifies the color received by a node
      var children: [1..0] int;         // list of neighbor ids
    }

    /** Abstract node representation */
    var nodeSet:[D] Node;
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
        root = reader.read(int);
        var line:string;

        for i in D {
            reader.read(line);
            for j in 0..(line.length-1) {
                adj_graph[i][j]=(line[j+1]:int);
            }
        }

        for i in D {
            parent[i]=reader.read(int);
        }

        reader.close();
        inFile.close();

        initialize();
        run();
        six2three();
        printOutput();

        if(verify) then
            outputVerifier();

        if(loadValue != 0) {
			var sumval: real = 0;
            for i in D{
                    sumval = sumval + nval[i];
            }
			if(sumval > 0) then
                writeln();
		}
    }

    /* Initializes all the fields of the abstract node. */
    proc initialize() {
       forall i in D {
            nlabel[i] = i;
            nodeSet[i] =  new Node();
            nodeSet[i].parent = parent[i];
            forall j in D {
                if(adj_graph[i][j] == 1 && nodeSet[i].parent != j) then
                    nodeSet[i].children.push_back(j);
            }
            nodeSet[i].color = nlabel[i];
        }
        colorLabel = log2(nodes);
        if(1<<colorLabel < nodes) then
            colorLabel+=1;
    }

    /*
        Aims to busy the threads by introducing the no-op instructions
        equivalent to the amount of load specified.
        :arg  weight:	Specifies the current load value for a thread.
        :type: int(64)
        :returns: Updated load value.
        :rtype: int(64)
    */
	proc loadweight(weight: int): int {
                var count=0;
                for i in {0..loadValue-1}{
                        count+=1;
                }
                return count+weight;
    }

    /* Runs the algorithm till the graph consists of atmost six colors. */
    proc run() {
        nodeSet[root].color = 0;
		do {
			sixColor();
		}while(checkAgain());
	}

    /* Determines the number of different colors used in the graph.
       :returns:    true if there is no color >= 6.
       :rtype: bool
    */
    proc checkAgain(): bool {
        var flag = false;
        forall i in D with (ref flag){
            if(again[i]) then
                flag=true;
        }
        return flag;
    }

    /* Reduces the number of colors used in the graph to six. */
    proc sixColor(){
        forall i in D {
            forall j in 1..nodeSet[i].children.size {
                sendColor(nodeSet[i].children[j], nodeSet[i].color);
            }

            if(loadValue != 0) then
                nval[i] = loadweight(nval[i]+i);
        }

        forall i in D {
            if(i!=root){
                again[i]=false;
                var xored = nodeSet[i].receivedColor ^ nodeSet[i].color;
                for k in {0..(colorLabel-1)}{
                    var pval = 1<<k;
                    var nand :int = xored & pval;
                    if(nand == pval) {
                        var nxored = nodeSet[i].color & pval;
                        if(nxored == 0) then
                            nodeSet[i].color = 2*k + 0;
                        else
                            nodeSet[i].color = 2*k + 1;
                        break;
                    }
                }

                if(nodeSet[i].color >= 6) then
                    again[i] = true;
            }

            if(loadValue != 0) then
                nval[i] = loadweight(nval[i]+i);
        }
    }


   /* Reduces the number of colors from six to three. */
   proc six2three() {
       for k in {1..3} {
            var x = 6-k;
            var randStream: RandomStream = new RandomStream(3);
            var ncolor : int = randStream.getNext(): int;
            ncolor = ncolor%3;
            shiftDown();
            if(nodeSet[root].color == ncolor) then
               ncolor = (ncolor+1)%3;
           nodeSet[root].color = ncolor;

           forall i in D {
                var cparent=0,cchild=0;
                   if(nodeSet[i].color == x) {
                       cparent=getColor(nodeSet[i].parent);
                       if(nodeSet[i].children.size >0) then
                           cchild=getColor(nodeSet[i].children[1]);
                       if(cparent+cchild == 1) then
                           nodeSet[i].color=2;
                       else if(cparent+cchild == 2) then
                           nodeSet[i].color=1;
                       else if(cparent+cchild == 3) {
                           if(cparent != 0 && cchild != 0) then
                               nodeSet[i].color=0;
                           else
                               nodeSet[i].color=1;
                       }
                       else if(cparent+cchild == 4) {
                           if(cparent != 0 && cchild != 0) then
                               nodeSet[i].color=0;
                           else
                               nodeSet[i].color=1;
                       }
                       else if(cparent+cchild == 5) {
                           if(cparent != 0 && cchild != 0) then
                               nodeSet[i].color=0;
                           else
                               nodeSet[i].color=1;
                       }
                       else
                           nodeSet[i].color = 0;
                   }

            if(loadValue != 0) then
                nval[i] = loadweight(nval[i]+i);
            }
        }

    }

   /* Shifts the color of parent down to its children. */
   proc shiftDown() {
        forall i in D {
            for j in {1..nodeSet[i].children.size} {
                    sendColor(nodeSet[i].children[j], nodeSet[i].color);
            if(loadValue != 0) then
                nval[i] = loadweight(nval[i]+i);
            }
        }
        forall i in D {
            if(i != root) then
                nodeSet[i].color = nodeSet[i].receivedColor;
            if(loadValue!=0) then
                nval[i] = loadweight(nval[i]+i);
        }
    }

    /* Provides the color of the ```aNode```.
	   :arg aNode: node whose color value is required.
       :type: int(64)
       :returns: color of ```aNode```.
       :rtype: int(64);
    */
    proc getColor(aNode:int):int {
        return nodeSet[aNode].color;
    }

    /* Sends the color of the parent node to a child node.
       :arg childNode: node whose color is to be changed
    */
    proc sendColor(childNode: int, acolor:int) {
        nodeSet[childNode].receivedColor = acolor;
    }

    proc printOutput() {
        var outfile = open(outputFile, iomode.cw);
        var writer = outfile.writer();
        for q in D {
            writer.writeln("Node " + q + ": \t Color " + nodeSet[q].color);
        }
        writer.close();
        outfile.close();
    }

    /* Validates the output resulting from the execution of the algorithm. */
    proc outputVerifier() {
        var count =0;
        var colormat : [D] int = -1;
        var flag = false;

        for i in D {
            if(colormat[nodeSet[i].color] <0) {
                colormat[nodeSet[i].color]=0;
                count+=1;
            }
        }
        if(count <= 3) {
            flag = false;
            for i in D {
                if(i != root){
                    if(nodeSet[i].color == nodeSet[nodeSet[i].parent].color) {
                        flag = true;
                        break;
                    }
                }

                for j in 1..nodeSet[i].children.size {
                    if(nodeSet[i].color == nodeSet[nodeSet[i].children[j]].color) {
                        flag = true;
                        break;
                    }
                }

            }
            if(!flag) then
                writeln("Output verified");
        }
    }
}
