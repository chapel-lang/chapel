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
config const copyfiles = false;
config const maxfiles = max(int);
config const buildset = true;
config const buildgraph = true;
config const localarray = true;
config const bulkadd = true;

param distributed = if CHPL_COMM == "none" then false
                    else true;

use FileSystem;
use Spawn;
use Time;
use Graph;
use Random;
use UserMapAssoc;
use BlockDist;
use List;
use listSorts;

// packing twitter user IDs to numbers
var total_tweets_processed : atomic int;
var total_lines_processed : atomic int;
var max_user_id : atomic int;

// Record of timers
var t: timings;

proc main(args:[] string) {
  var files = args[1..];
  var todo:list(string);

  for arg in files {
    if isDir(arg) {
      // Go through files in directories.
      for f in findfiles(arg, true) {
        if todo.size < maxfiles then todo.append(f);
      }
    } else {
      if todo.size < maxfiles then todo.append(arg);
    }
  }

  t.total.start();

  // Pairs is for collecting twitter  user ID to user ID mentions
  var assocDist = new UserMapAssoc(idxType=(int, int), mapper=new MinMapper()),
      noDist = new DefaultDist();

  var Dist = if distributed then assocDist
             else noDist;

  var Pairs: domain( (int, int) ) dmapped new dmap(Dist);

  run(todo, Pairs);

  t.total.stop();

  if timing then
    t.print();
}



proc run(todo:list(string), Pairs) {
  const FilesSpace = {1..todo.length};
  const BlockSpace = if distributed then
                       FilesSpace dmapped Block(boundingBox=FilesSpace)
                     else
                       FilesSpace;
  var allfiles:[BlockSpace] string;
  allfiles = todo;

  todo.destroy();

  if copyfiles {
    t.copy.start();
    forall f in allfiles {
      on f {
        var from = f;
        var base = basename(f);
        var uname:c_string;
        sys_getenv(c"USER", uname);
        var to = "/tmp/" + uname + base;
        if progress then writeln("locale ", here.id, " copying ", from, " to ", to);
        //copy(from, to);
        var sub = spawn(["cp", from, to]);
        sub.wait();
        if progress then writeln("locale ", here.id, " done copying");
        f = to;
      }
    }
    t.copy.stop();
  }



  if buildset then
    writeln("parse+set build on ", numLocales, " locales");
  else
    writeln("parse on ", numLocales, " locales");

  t.parse.start();
  forall f in allfiles {
    // TODO -- performance/scalability
    // we don't want to lock/unlock the hashtable
    // every time we add an entry. Appends could
    // be aggregated.

    const fname_copy = f; // avoids problems with \0 missing
    process_json(fname_copy, Pairs);

    //process_json(f, Pairs);
  }
  t.parse.stop();

  writeln("done parsing");
  if copyfiles {
    for f in allfiles {
      on f {
        if progress then writeln("locale ", here.id, " removing ", f);
        const fname_copy = f; // avoids problems with \0 missing
        var err:syserr;
        remove(error=err, fname_copy);
      }
    }
  }

  if verbose {
    if distributed {
      // TODO -- this is not portable code
      // need UserMapAssoc to support localDomain
      for i in 0..#numLocales {
        writeln("on locale ", i, " there are ",
                Pairs._value.locDoms[i].myInds.size,
                " values");
      }
    }
  }

  writeln("Pairs.size is ", Pairs.size);

  if verbose then
    writeln("the maximum user id was ", max_user_id.read());

  if buildgraph then
    create_and_analyze_graph(Pairs);
}

record TweetUser {
  var id:int;
}

record TweetEntities {
  // read somethig like user_mentions=[1, 3, 4, 5]
  //var user_mentions:[1..0] TweetUser; // TODO - support this
  var user_mentions:list(TweetUser);
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


proc process_json(logfile:channel, fname:string, Pairs) {
  var tweet:Tweet;
  var empty:Empty;
  var err:syserr;
  var got:bool;

  var ntweets = 0;
  var nlines = 0;
  var max_id = 0;

  if progress then
    writeln(fname, " : processing");

  //var localPairs: domain( (int, int), parSafe=false );
  var localPairs: [1..0] (int, int);

  while true {
    got = logfile.readf("%~jt", tweet, error=err);
    if got && !err {
      if verbose then writef("%jt\n", tweet);
      var id = tweet.user.id;
      if max_id < id then max_id = id;
      for mentions in tweet.entities.user_mentions {
        var other_id = mentions.id;
        if max_id < other_id then max_id = other_id;
        // Add (id, other_id) to Pairs,
        // but leave out self-mentions
        if id != other_id {
          if buildset {
            //localPairs += (id, other_id);
            if localarray {
              localPairs.push_back( (id, other_id) );
            } else {
              Pairs += (id, other_id);
            }
          }
        }
      }
      ntweets += 1;
    }
    if err == EFORMAT {
      if verbose then
          stdout.writeln("error reading tweets ", fname, " offset ",
            logfile.offset(), " : ", errorToString(err));

      // read over something else
      got = logfile.readf("%~jt", empty, error=err);
    }
    if err == EEOF then break;
    if err {
      stderr.writeln("severe error reading tweets ", fname, " offset ",
          logfile.offset(), " : ", errorToString(err));

      halt("ERROR");

      // advance to the next line.
      logfile.readln();
    }
    nlines += 1;
  }

  logfile.close();

//  for p in localPairs.sorted(new DestinationComparator()) {
//    Pairs += p;
//  }

  if localarray {
    if bulkadd {
      quickSort(localPairs, comparator=new DestinationComparator());
      Pairs._value.dsiAddSorted(localPairs);
    } else {
      quickSort(localPairs, comparator=new DestinationComparator());
      for p in localPairs do
        Pairs += p;
    }
  }

  if progress then
    writeln(fname, " : ",
            ntweets, " tweets / ", nlines, " lines");

  total_tweets_processed.add(ntweets);
  total_lines_processed.add(nlines);

  while true {
    var got = max_user_id.read();
    var id = if got > max_id then got else max_id;
    var success = max_user_id.compareExchangeWeak(got, id);
    if success then break;
  }
}

proc process_json(fname: string, Pairs)
{

  if progress then writeln("locale ", here.id, " in process_json file is ", fname);

  var last3chars = fname[fname.length-2..fname.length];
  if last3chars == ".gz" {
    //var sub = spawn(["gunzip", "-c", fname], stdout=PIPE);
    //process_json(sub.stdout, fname, Pairs);

    var nogz = fname[1..fname.length-3];

    var base = basename(nogz);
    var uname:c_string;
    sys_getenv(c"USER", uname);
    var to = "/tmp/" + uname + base;

    var sub = spawnshell("gunzip --stdout " + fname + " > " + to);
    sub.wait();
 
    var fi = open(to, iomode.r);
    var logfile = fi.reader();
    process_json(logfile, fname, Pairs);
    fi.close();

    remove(to);

  } else {
    var fi = open(fname, iomode.r);
    var logfile = fi.reader();
    process_json(logfile, fname, Pairs);
    fi.close();
  }
}


record Triple {
  var from: int(32);
  var to: int(32);
  proc weight return 1:int(32);
}


proc create_and_analyze_graph(Pairs)
{
  if true {
    writeln("finding mutual mentions");

    writeln(Pairs.size, " pairs / ",
            total_tweets_processed.read(), " tweets / ",
            total_lines_processed.read(), " lines");
  }

  t.graph.start();

  t.mutualmentions.start();
  // Build idToNode
  var userIds:domain(int);

  var nmutual = 0;
  forall (id, other_id) in Pairs with (ref userIds, + reduce nmutual) {
    if Pairs.member( (other_id, id) ) {
      //writeln("Reciprocal match! ", (id, other_id) );

      // add to userIds
      userIds += id;
      // Not necessary to add other_id now since we will
      // add it when processing (other_id, id)
      if id < other_id then
        nmutual += 1;
    }
  }
  t.mutualmentions.stop();

  writeln("total mutual mentions ", nmutual);

  if progress then
    writeln(userIds.size, " users");

  if progress then
    writeln("compacting ids");

  t.compactids.start();
  // compact userIds into idToNode
  var idToNode: [userIds] int(32);
  var nodeToId: [1..userIds.size] int;

  // reduction intent would help here to compute max node id
  // TODO -- bug: this loop is not working with a forall (compile error)
  for (node, id) in zip(1..userIds.size, userIds.sorted()) {
    idToNode[id] = node:int(32);
    nodeToId[node] = id;
  }
  t.compactids.stop();

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

  t.createtriples.start();


  // TODO - this is very slow:
  /*
  var triples = [(id, other_id) in Pairs]
                   if id < other_id && Pairs.member( (other_id, id) ) then
                     new Triple(idToNode[id], idToNode[other_id]);
                     */

  // TODO - this is still slow, but it's faster
  var idx : atomic int;
  idx.write(1);
  var triples:[1..#nmutual] Triple;
  forall (id, other_id) in Pairs {
    if id < other_id && Pairs.member( (other_id, id) ) {
      var got_idx = idx.fetchAdd(1, memory_order_relaxed);
      triples[got_idx] = new Triple(idToNode[id], idToNode[other_id]);
    }
  }

  // TODO - would like to write it with a zip of assoc and non-assoc
  // (but then where would be the conditional? Adding dummy nodes e.g.?)

  // A parallel scan might also work... but scan is currently serial.

  t.createtriples.stop();

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


  t.maxnid.start();
  // TODO: This line wouldn't be necessary if we had reduce intents
  max_nid = max reduce [r in triples] max(r.from, r.to);
  t.maxnid.stop();

  if progress then
    writeln("creating graph");

  t.buildgraph.start();
  // TODO - performance - merge graph element updates
  var G = buildUndirectedGraph(triples, false, {1..max_nid} );

  // 'gt' is defined in graph module and populater after a graph build
  t.buildgraphcomponents = gt;

  // Clear out memory used by triples.
  triples.domain.clear();

  t.buildgraph.stop();

  t.graph.stop();

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
    writeln("connected component analysis");

  t.cca.start();

  // Component ID
  var cid: int(32) = 0;
  // Graph of componenent IDs
  var cids:[G.vertices] int(32);
  // todo stack
  var todo: list(int(32));

  for vid in G.vertices {
    // if vertex has not been visited
    if cids[vid] then continue;

    cid += 1;
    todo.prepend(vid: int(32));

    if printall then
      writeln('Vertex ', vid, ' has neighbors:');

    do {
      for nid in G.Neighbors(todo.pop_front()) {
        // TODO -- checking value of nid shouldn't be necessary
        if nid == 0 then continue;
        if cids[nid] then continue;
        cids[nid] = cid;
        todo.prepend(nid:int(32));

        if printall then
          writeln(nid);

      }
    } while todo.first != nil;
  }

  const totalcomponents = cid;

  if verbose then
    writeln("number of connected components: ", totalcomponents);

  t.cca.stop();


  if progress then
    writeln("graph partition");

  t.partition.start();

  // TODO -- parallelize (should be easy)
  var components: [{1..totalcomponents}] list(int(32));
  for vid in G.vertices {
    components[cids[vid]].append(vid);
  }


  var partitionedGraph = partition_graph(components);

  if verbose {
    writeln("partition sizes:");
    for bin in partitionedGraph{
      writeln(bin.Array.size);
    }
  }

  t.partition.stop();


  if progress then
    writeln("label propagation");

  t.labelprop.start();

  // start with a different label for each node
  // the labels correspond to clusters.

  var labels:[1..max_nid] atomic int(32);
  forall i in 1..max_nid {
    labels[i].poke(i);
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
  go.write(true, memory_order_relaxed);

  var i = 0;

  while go.read(memory_order_relaxed) && i < maxiter {
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
    go.write(false, memory_order_relaxed);

    /* for Block Distributed version, we need to:

        * Find a way to dmap with out specific bins from above
          * 1xnumLocales array of arrays?
          * coforall { on loc { }}?

        * Make sure all other read/writes are local
          * Encapsulate inner loop
          * local copies of:
            * labels?
            * G (read-only)

        * After all is said and done, increment labels by largest label from
          previous locale

        * I assume reorder reduces overlap of neighbors?

    */
    // TODO:  -> forall, but handle races in vertex labels?
    // iterate over G.vertices in a random order
    serial !parallel { forall partition in partitionedGraph{
    //for vid in G.vertices {
      serial !parallel { forall vid in partition.Array {

        //if distributed && verbose {
          //writeln("on locale ", here.id, " there are ",
          //        distributedVertices._value.locArr[here.id].myElems.size,
          //        " vertices");
        //}

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
          var nlabel = labels[nid].read(memory_order_relaxed);
          if printall then
            writeln("with label ", nlabel);

          if ! foundLabels.member(nlabel) {
            foundLabels += nlabel;
          }
          counts[nlabel] += 1;
        }

        var mylabel = labels[vid].read(memory_order_relaxed);

        // TODO: ties should be broken uniformly randomly
        var maxlabel:int(32) = 0;
        var maxcount = 0;
        // TODO -- performance -- this allocates memory.
        // There might not be a tie.
        var tiebreaker = makeRandomStream(seed+vid, eltType=bool,
                                          parSafe=false, algorithm=RNG.PCG);
        for (count,lab) in zip(counts, counts.domain) {
          if count > maxcount || (count == maxcount && tiebreaker.getNext()) {
            maxcount = count;
            maxlabel = lab;
          }
        }
        delete tiebreaker;

        // TODO:
        // Did the existing label correspond to a maximal label?
        // stop when every node has a label a maximum number of neighbors have
        // (e.g. there might be 2 labels each attaining the maximum)
        if foundLabels.member[mylabel] && counts[mylabel] < maxlabel {
          go.write(true, memory_order_relaxed);
        }

        // set the current label to the maximum label.
        if mylabel != maxlabel then
          labels[vid].write(maxlabel, memory_order_relaxed);
      } // forall vid
      } // serial !parallel
    } // forall partition
    i += 1;
    } // serial !parallel
  } // while !go

  t.labelprop.stop();


  if output {
    for vid in G.vertices {
      writeln("twitter user ", nodeToId[vid],
              " is in group ",
              nodeToId[labels[vid].read(memory_order_relaxed)]);
    }
  }
}

/* Partition graph vertices into bins for distribution across locales */
proc partition_graph(components) {
  // TODO -- Replace with real numLocales at some point
  const perLocale= LocaleSpace dmapped Block(LocaleSpace);
  var bins: [perLocale] Sub(int(32));

  QuickSortLists(components, reverse=true);

  // Populate bins
  for i in perLocale {
    bins[i] = new Sub(int(32), {1..0});
  }

  // Distribute array values across bins
  for idxlist in components {
    for idx in idxlist do
      bins[0].Array.push_back(idx);
    first_sort(bins);
  }

  return bins;
}

/* Sub-region of a domain/array */
class Sub {
  type eltType;
  // TODO -- should these be associative domain/arrays?
  var Domain: domain(1);
  var Array: [Domain] eltType;
}

class MinMapper{
  proc indexToLocaleIndex(ind, targetLocs: [] locale) : int {
    var (x, y) = ind;
    var m = min(x, y);
    var h: int = _gen_key(m);
// This statement runs a fence because of domain
// reference counting. Boo.
//    const numlocs = targetLocs.domain.size;
    const numlocs = targetLocs.size;
    return h % numlocs;
  }
}

record DestinationComparator{
  proc key(p) {
    var (x,y) = p;
    var m = min(x,y);
    var h: int = _gen_key(m);
    return (h+here.id) % numLocales;
  }
}

/* Selection sort that assumes that only the first element is out of order */
proc first_sort(Data: [?Dom] ?eltType) where Dom.rank == 1 {
  for j in Dom {
    var iMin = j;
    for i in j+1..Dom.high {
      if Data[i].Array.size < Data[iMin].Array.size then
        iMin = i;
    }

    if (iMin != j) then
      Data[j] <=> Data[iMin];
    else
      break;
  }
}

record timings {
  var total, copy, parse, graph, mutualmentions, compactids, createtriples, maxnid, buildgraph, cca, partition, labelprop: Timer;
  var buildgraphcomponents: graphtimings;
}

/* Consolidate timings prints to one function */
proc timings.print() {
  var nlines = total_lines_processed.read(),
        days = total.elapsed(TimeUnits.hours) / 24.0,
           m = 1000000.0;

  writeln("Timings:");
  if copyfiles then writeln("copy files in ", copy.elapsed(), " s");
  writeln("parsed ", nlines, " lines in ", parse.elapsed(), " s");
  writeln("that's ", nlines / parse.elapsed() / 1000.0, " K tweets/s");
  writeln("created graph in   ", graph.elapsed(), " s");
  writeln("graph components:");
  writeln("  mutual mentions: ", mutualmentions.elapsed(), " s");
  writeln("  compact ids    : ", compactids.elapsed(), " s");
  writeln("  create triples : ", createtriples.elapsed(), " s");
  writeln("  max nid        : ", maxnid.elapsed(), " s");
  writeln("  build graph    : ", buildgraph.elapsed(), " s");
  buildgraphcomponents.print();
  writeln("connected component analysis ran in ", cca.elapsed(), " s");
  writeln("graph partitioning ran in ", partition.elapsed(), " s");
  writeln("label propagation ran in ", labelprop.elapsed(), " s");

  writeln("processed ", nlines, " lines in ", total.elapsed(), " s");
  writeln("that is ", nlines / days / m, "M tweets/day processed");
}
