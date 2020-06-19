/*

Label Propagation for Community Detection

see CUG’15 paper: Implementing a social-network analytics pipeline using
Spark on Urika XA

Computation consists of these steps:
* Read in gzip files storing JSON-encoded tweets
* Find pairs of Twitter users that @mention each other
* Construct a graph from such users
* Run a label propagation algorithm on that graph
* Output the community structure resulting from label propagation

*/

/* How can we represent a graph in Chapel?
    * Matrix view - dense or sparse
    * Class instances tree

    * Opaque Domain view
    var People = opaque;
    var Child: [People] [1..maxChildren] index(People);


    * Associative Domain view

    var People = domain(int);
    var Child: [People] [1..maxChildren] index(People);

    * Array of Edges view
     -> rectangular array of Vertex by vertex ID
     -> each Vertex contains an array of edges (a bag of values)

    We don't yet have opaque distributions or associative distributions.
      -- see test/distributions/bradc/assoc

    We don't yet support inner arrays with different sizes.

 */

config const progress = false;
config const timing = false;
config const verbose = false;
config const printall = false;
config var seed = 0;
config const maxiter = 20;
config const output = true;
config const parallel = true;
config param distributed = false; // NOTE - could default to CHPL_COMM != none

use FileSystem;
use Spawn;
use Time;
use IO;
use Graph;
use Random;
use HashedDist;
use LinkedLists;
use BlockDist;

// packing twitter user IDs to numbers
var total_tweets_processed : atomic int;
var total_lines_processed : atomic int;
var max_user_id : atomic int;

proc main(args:[] string) {
  var files = args[1..];
  var todo:LinkedList(string);

  for arg in files {
    if isDir(arg) {
      // Go through files in directories.
      for f in findfiles(arg, true) {
        todo.append(f);
      }
    } else {
      todo.append(arg);
    }
  }

  // TODO - using 2 functions because of lack of
  // domain assignment in Hashed
  // Pairs is for collecting twitter  user ID to user ID mentions
  if distributed {
    var Pairs: domain( (int, int) ) dmapped Hashed(idxType=(int, int));
    run(todo, Pairs);
  } else {
    var Pairs: domain( (int, int) );
    run(todo, Pairs);
  }
}



proc run(ref todo:LinkedList(string), ref Pairs) {
  var t:Timer;
  t.start();

  const FilesSpace = {1..todo.size};
  const BlockSpace = if distributed then
                       FilesSpace dmapped Block(boundingBox=FilesSpace)
                     else
                       FilesSpace;
  var allfiles:[BlockSpace] string;
  allfiles = todo;

  todo.destroy();

  var parseAndMakeSetTime:Timer;
  parseAndMakeSetTime.start();


  forall f in allfiles with (ref Pairs) {
    // TODO -- performance/scalability
    // we don't want to lock/unlock the hashtable
    // every time we add an entry. Appends could
    // be aggregated.
    process_json(f, Pairs);
  }

  parseAndMakeSetTime.stop();

  if verbose {
    if distributed {
      // TODO -- this is not portable code
      // need HashedDist to support localDomain
      for i in 0..#numLocales {
        writeln("on locale ", i, " there are ",
                Pairs._value.locDoms[i].myInds.size,
                " values");
      }
    }
  }

  if verbose then
    writeln("the maximum user id was ", max_user_id.read());

  var nlines = total_lines_processed.read();

  if timing {
    writeln("parsed ", nlines, " lines in ", parseAndMakeSetTime.elapsed(), " s ");
  }

  create_and_analyze_graph(Pairs);

  t.stop();
  var days = t.elapsed(TimeUnits.hours) / 24.0;
  var m = 1000000.0;
  if timing {
    writeln("processed ", nlines, " lines in ", t.elapsed(), " s ");
    writeln("that is ", nlines / days / m, "M tweets/day processed");
  }
}

record TweetUser {
  var id:int;
}

record TweetEntities {
  // read somethig like user_mentions=[1, 3, 4, 5]
  //var user_mentions:[1..0] TweetUser; // TODO - support this
  var user_mentions:LinkedList(TweetUser);
}

record User {
  var id:int;
}

record Tweet {
  var id:int;
  var user:User;
  var entities:TweetEntities;
}

record Empty {
}


proc process_json(logfile:channel, fname:string, ref Pairs) {
  var tweet:Tweet;
  var empty:Empty;
  var got:bool;

  var ntweets = 0;
  var nlines = 0;
  var max_id = 0;

  if progress then
    writeln(fname, " : processing");

  while true {
    try! {
      try {
        got = logfile.readf("%~jt", tweet);
      } catch e: BadFormatError {
        if verbose then
            stdout.writeln("error reading tweets ", fname, " offset ",
              logfile.offset(), " : ", errorToString(e.err));

        // read over something else
        got = logfile.readf("%~jt", empty);
      }
    } catch e: SystemError {
      stderr.writeln("severe error reading tweets ", fname, " offset ",
          logfile.offset(), " : ", errorToString(e.err));

      // advance to the next line.
      logfile.readln();
    } // halt on truly unknown error

    if got {
      if verbose then writef("%jt\n", tweet);
      var id = tweet.user.id;
      if max_id < id then max_id = id;
      for mentions in tweet.entities.user_mentions {
        var other_id = mentions.id;
        if max_id < other_id then max_id = other_id;
        // Add (id, other_id) to Pairs,
        // but leave out self-mentions
        if id != other_id {
          Pairs += (id, other_id);
        }
      }
      ntweets += 1;
    } else { // end of file
      break;
    }

    nlines += 1;
  }

  logfile.close();

  if progress then
    writeln(fname, " : ",
            ntweets, " tweets / ", nlines, " lines");

  total_tweets_processed.add(ntweets);
  total_lines_processed.add(nlines);

  while true {
    var got = max_user_id.read();
    var id = if got > max_id then got else max_id;
    var success = max_user_id.compareAndSwap(got, id);
    if success then break;
  }
}

proc process_json(fname: string, ref Pairs)
{

  var last3chars = fname[fname.size-3..];
  if last3chars == ".gz" {
    var sub = spawn(["gunzip", "-c", fname], stdout=PIPE);
    process_json(sub.stdout, fname, Pairs);
  } else {
    var logfile = openreader(fname);
    process_json(logfile, fname, Pairs);
  }
}


record Triple {
  var from: int(32);
  var to: int(32);
  proc weight return 1:int(32);
}


proc create_and_analyze_graph(Pairs)
{
  if progress {
    writeln("finding mutual mentions");

    writeln(Pairs.size, " pairs / ",
            total_tweets_processed.read(), " tweets / ",
            total_lines_processed.read(), " lines");
  }

  var createGraphTime:Timer;
  createGraphTime.start();

  var nmutual = 0;

  // Build idToNode
  var userIds:domain(int);

  forall (id, other_id) in Pairs with (ref userIds) {
    if Pairs.contains( (other_id, id) ) {
      //writeln("Reciprocal match! ", (id, other_id) );

      // add to userIds
      userIds += id;
      // Not necessary to add other_id now since we will
      // add it when processing (other_id, id)
    }
  }

  if progress then
    writeln(Pairs.size, " pairs / ",
            total_tweets_processed.read(), " tweets / ",
            total_lines_processed.read(), " lines");

  if progress then
    writeln("compacting ids");

  // compact userIds into idToNode
  var idToNode: [userIds] int(32);
  var nodeToId: [1..userIds.size] int;

  // reduction intent would help here to compute max node id
  // TODO -- bug: this loop is not working with a forall (compile error)
  for (node, id) in zip(1..userIds.size, userIds.sorted()) {
    idToNode[id] = node:int(32);
    nodeToId[node] = id;
  }

  if printall {
    writeln("idToNode:");
    for id in userIds {
      writeln(id, " -> ", idToNode[id]);
    }
    writeln("nodeToId:");
    for id in 1..userIds.size {
      writeln(id, " -> ", nodeToId[id]);
    }
  }

  // construct array of triples (from, to, weight)
  // this is also known as Coordinate List (COO)
  var max_nid:int(32);

  if progress then
    writeln("creating triples");

  var triples = [(id, other_id) in Pairs]
                   if id < other_id && Pairs.contains( (other_id, id) ) then
                     new Triple(idToNode[id], idToNode[other_id]);

  if printall {
    writeln("triples are:");
    for t in triples {
      writeln(t);
    }
  }

  // Pairs are no longer needed.
  // Clear out memory used by pairs.
  Pairs.clear();

  // Now construct graph using nodes namespace (vs user ids one)
  // that enables the graph to use rectangular arrays instead
  // of hashtables.


  // TODO: This line wouldn't be necessary if we had reduce intents
  max_nid = max reduce [r in triples] max(r.from, r.to);

  if progress then
    writeln("creating graph");

  // TODO - performance - merge graph element updates
  var G = buildUndirectedGraph(triples, false, {1..max_nid} );

  // Clear out memory used by triples.
  triples.domain.clear();

  createGraphTime.stop();

  if timing {
    writeln("created graph in ", createGraphTime.elapsed(), " s ");
  }

  if progress then
    writeln("done creating graph");

  if printall {
    for v in G.vertices {
      writeln("Vertex ", v);
      for nid in G.Neighbors(v) {
        writeln("edge to ", nid); //, " weight ", weight);
      }
    }
  }

  if progress then
    writeln("label propagation");

  var graphAlgorithmTime:Timer;
  graphAlgorithmTime.start();

  // start with a different label for each node
  // the labels correspond to clusters.

  var labels:[1..max_nid] atomic int(32);
  forall (lab,i) in zip(labels,1:int(32)..) {
    // TODO -- elegance - use "atomic counter" that acts as normal var
    // or change the default for the atomic
    labels[i].write(i, memoryOrder.relaxed);
  }

  // label propagation for community detection according to
  // http://arxiv.org/pdf/0709.2938.pdf
  // "Near linear time algorithm to detect community structures
  //  in large-scale networks"
  // Raghavan et al.

  // go through vertices in a random order
  // random order -> we can go through them with a forall
  //   over an arbitrary remapping
  // TODO -- shouldn't actually need to compute a permutation;
  // we could be computing a random iteration somehow...
  // (it could be with specialized 'i' or 'once' generator from PCG,
  //  since that generates each output exactly once.)
  // go through random numbers r(i)
  //  work on corresponding element r(i) if r(i) is in range

  // TODO -- perhaps the algorithm should recompute the
  // permutation each iteration through the big while loop?

  // re-seed the RNG if seed is 0.
  if seed == 0 {
    seed = SeedGenerator.currentTime;
  }


  // TODO - performance - use a PCG once-only RNG in order to
  //  iterate through all of these values.
  //  To do that, we'd want to have an iterator yielding once
  //  each value in a range. That would be implemented by:
  //   - having a yield-once generator for each power of 2
  //     (these correspond to PCG 'insecure' generators
  //      with as much internal state as the yielded values)
  //   - yield values only within the requested range
  //     (in order to yield once, keep statistics)
  var reorder:[G.vertices] int(32);
  permutation(reorder, seed=seed);
  // Or we could just do it in the normal order...

  var go: atomic bool;
  go.write(true, memoryOrder.relaxed);

  var i = 0;

  while go.read(memoryOrder.relaxed) && i < maxiter {
    // TODO: brad recommends changing the above to a for
    // look and then adding a break. He suggests:
    /*
      for i in 1.. {
        if (stop) then
          break;
        stop = true;
        forall ... {
          if needs more work {
            stop = false;
        }
      }
    */

    // stop unless we determine we should continue
    go.write(false, memoryOrder.relaxed);

    // TODO:  -> forall, but handle races in vertex labels?
    // iterate over G.vertices in a random order
    serial !parallel { forall vid in reorder {
    //for vid in G.vertices {

      if printall then
        writeln("on node ", vid, " currently in group ", labels[vid]);

      // label -> count
      var foundLabels:domain(int(32));
      var counts:[foundLabels] int;

      for nid in G.Neighbors(vid) {
        // TODO -- shouldn't be needed.
        if nid == 0 then continue;

        if printall then
          writeln("on neighbor ", nid);
        var nlabel = labels[nid].read(memoryOrder.relaxed);
        if printall then
          writeln("with label ", nlabel);

        if ! foundLabels.contains(nlabel) {
          foundLabels += nlabel;
        }
        counts[nlabel] += 1;
      }

      var mylabel = labels[vid].read(memoryOrder.relaxed);

      // TODO: ties should be broken uniformly randomly
      var maxlabel:int(32) = 0;
      var maxcount = 0;
      // TODO -- performance -- this allocates memory.
      // There might not be a tie.
      var tiebreaker = createRandomStream(seed+vid, eltType=bool,
                                        parSafe=false, algorithm=RNG.PCG);
      for (count,lab) in zip(counts, counts.domain) {
        if count > maxcount || (count == maxcount && tiebreaker.getNext()) {
          maxcount = count;
          maxlabel = lab;
        }
      }

      // TODO:
      // Did the existing label correspond to a maximal label?
      // stop when every node has a label a maximum number of neighbors have
      // (e.g. there might be 2 labels each attaining the maximum)
      if foundLabels.contains[mylabel] && counts[mylabel] < maxlabel {
        go.write(true, memoryOrder.relaxed);
      }

      // set the current label to the maximum label.
      if mylabel != maxlabel then
        labels[vid].write(maxlabel, memoryOrder.relaxed);
    }
    i += 1;
  } }

  graphAlgorithmTime.stop();

  if timing {
    writeln("graph algorithm ran ", graphAlgorithmTime.elapsed(), " s ");
  }


  if output {
    for vid in G.vertices {
      writeln("twitter user ", nodeToId[vid],
              " is in group ",
              nodeToId[labels[vid].read(memoryOrder.relaxed)]);
    }
  }

  delete G;
}


