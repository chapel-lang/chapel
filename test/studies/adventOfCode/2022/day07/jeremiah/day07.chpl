use IO, Map, List;

// for part 1
const MaxDirSize = 100000;

//for part 2
const TotalMemSize = 70000000,
      SizeNeeded = 30000000;

config const verbose = false;

enum EntryKind {
    Dir,
    File
}

class Entry {
    var kind: EntryKind;
    var size: int;
    var name: string;
    var contents: map(string, shared Entry);

    proc type newDir(name: string) {
        var e = new shared Entry();
        e.kind = EntryKind.Dir;
        e.size = 0;
        e.name = name;
        return e;
    }

    proc type newFile(name: string, size: string) {
        var e = new shared Entry();
        e.kind = EntryKind.File;
        e.size = size:int;
        e.name = name;
        return e;
    }
}

var dirsRoot = parseDirStructure();

// part 1
writeln("total target sizes: ", sumTargetDirSizes(dirsRoot));

// part 2;
const stillNeeded = SizeNeeded - (TotalMemSize - dirsRoot.size);
writeln("smallest sufficient dir size: ", smallestSufficientDir(dirsRoot, stillNeeded));

proc parseDirStructure() {
    var line: string,
        root = Entry.newDir("root"),
        workingDirStack = new list(shared Entry);
    workingDirStack.append(root);

    while readLine(line, stripNewline=true) {
        const (cmd_dir_size, _, name) = line.partition(" ");
        select cmd_dir_size {
            when "$" {
                select name {
                    when "ls" {
                        if verbose {
                            write("\t" * workingDirStack.size);
                            writeln("parsing: ", workingDirStack.last().name);
                        }
                    }
                    otherwise {
                        const (_, _, toDir) = name.partition(" ");
                        select toDir {
                            when "/" do writeln("Parsing Dirs...");
                            when ".." do workingDirStack.pop();
                            otherwise do workingDirStack.append(
                                workingDirStack.last().contents.getValue(toDir));
                        }
                    }
                }
            }
            when "dir" {
                if verbose {
                    write("\t" * workingDirStack.size);
                    writeln("(dir) ", name);
                }
                workingDirStack.last().contents.add(
                    name,
                    Entry.newDir(name)
                );
            }
            otherwise {
                if verbose {
                    write("\t" * workingDirStack.size);
                    writeln("(file) ", name, " : ", cmd_dir_size);
                }
                workingDirStack.last().contents.add(
                    name,
                    Entry.newFile(name, cmd_dir_size)
                );
            }
        }
    }

    computeDirSizes(root);
    return root;
}

proc computeDirSizes(e: shared Entry) : int {
    select e.kind {
        when EntryKind.Dir {
            e.size = + reduce computeDirSizes(e.contents.values());
            return e.size;
        }
        when EntryKind.File do return e.size;
        otherwise return 0;
    }
}

// for part 1
proc sumTargetDirSizes(e: shared Entry): int {
    if e.kind == EntryKind.Dir {
        const subSizes = + reduce sumTargetDirSizes(e.contents.values());
        return if e.size < MaxDirSize then e.size + subSizes else subSizes;
    } else {
        return 0;
    }
}

// for part 2
proc smallestSufficientDir(e: shared Entry, needed: int): int {
    const best = min reduce smallestSufficientDir(e.contents.values(), needed);
    if e.kind == EntryKind.Dir {
        return if e.size < best && e.size > needed then e.size else best;
    }
    return best;
}
