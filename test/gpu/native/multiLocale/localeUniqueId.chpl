use Set;

var idset = new set(localeUID);
var gpu_counter = 0;

for loc in Locales {
    idset.add(loc.uid);

    for gpuLoc in loc.gpus {
        idset.add(gpuLoc.uid);
        gpu_counter += 1;
    }
}

writeln(idset.size == gpu_counter + numLocales);
