module ResultDB {
    private use List;
    private use SortedMap;
    use IO.FormattedIO;

    record ResultDatabase {
        /*
        Map the results to their values
        Each of the two (or more) test metrics (bandwidth, flops ,etc) maintain
        their own database.

        We maintain a map of block sizes to a list of the units
        Since each metric has its own database and the units remain the same
        for that metric, units is an attribute of the database.

        This way we minimize space needed and improve access times
        when we calculate the stats.
        */

        var testName: string;
        var units: string;
        var atts: string;
        var attsSuffix: string;
        var results: sortedMap;
        var homeLocale: locale;

        proc init(testName: string, units: string, atts = "BlckSz:", attsSuffix = "KB") {
            this.testName = testName;
            this.units = units;
            this.atts = atts;
            this.attsSuffix = attsSuffix;
            this.results = new sortedMap(string, list(real));
            this.homeLocale = here;
        }

        proc addToDatabase(blockSize: string, value: real){
            on homeLocale {
                // Since the [] Access adds the mapping if it does not
                // exist we do not have to initialize a new list when one
                // does not exist which is neat
                this.results[blockSize].pushBack(value);
            }
        }

        proc printDatabaseStats() {
            on homeLocale {
                if(this.results.isEmpty()){
                    writeln("Map Empty");
                }
                else {
                    printStatHeaders();

                    // For each blockSize calculate stats
                    for blockSize in this.results {
                        // Calc median
                        var median: real;
                        var resultSorted = this.results[blockSize];
                        resultSorted.sort();
                        // writeln(resultSorted.indices);
                        if(resultSorted.size % 2 == 0){
                            median = resultSorted[(resultSorted.size-1)/2]; // -1 to get the actual last index
                                                                            // because of  0-based indexing
                        } else {
                            median = (resultSorted[(resultSorted.size)/2] +
                                        resultSorted[(resultSorted.size-2)/2]) / 2 ;
                        }

                        // Calc mean
                        var sum :real = 0;
                        for res in this.results[blockSize] do sum += res;
                        var mean = sum/resultSorted.size;


                        // Calc Std dev
                        // stddev = sqrt((sum of all ((x-mean)^2))/size)
                        var sumOfSqDiff: real;
                        for res in this.results[blockSize] do sumOfSqDiff += (res-mean)**2;
                        var stdDev = (sumOfSqDiff/this.results[blockSize].size)**0.5;

                        // Calc min and max
                        var min = resultSorted.first();
                        var max = resultSorted.last();

                        // Write the result row
                        write(testName,"\t", atts, blockSize, attsSuffix, "\t", units,'\t');
                        try! write("%7.4dr".format(median), '\t',
                            "%7.4dr".format(mean), '\t',
                            "%7.4dr".format(stdDev), '\t',
                            "%7.4dr".format(min), '\t',
                            "%7.4dr".format(max), '\t');
                        // write(median, '\t', mean, '\t', stdDev, '\t', min, '\t', max, '\t' );
                        // Write all runs
                        for res in this.results[blockSize] {
                            try! write("\t", "%7.4dr".format(res));
                            // write('\t', res );
                        }
                        writeln();
                    }

                }
            }
        }

        proc printStatHeaders(){
            on homeLocale {
                if(this.results.isEmpty()){
                    writeln("Map Empty");
                }
                else {
                    write("\nTest\t\tAttributes\tUnits\t Median\t Mean\t StdDev\t Min\t Max\t");

                    // Get number of trials and print em
                    for idx in 0..<this.results[this.results.keysToArray()[0]].size{
                        write("\t Trial", idx);
                    }
                    writeln();
                }
            }
        }

        proc printPerfStats(){
            on homeLocale {
                if(this.results.isEmpty()){
                    writeln("Map Empty");
                }
                else {
                    // For each blockSize calculate stats
                    for blockSize in this.results {
                        // Calc median
                        var median: real;
                        var resultSorted = this.results[blockSize];
                        resultSorted.sort();
                        // writeln(resultSorted.indices);
                        if(resultSorted.size % 2 == 0){
                            median = resultSorted[(resultSorted.size-1)/2]; // -1 to get the actual last index
                                                                            // because of  0-based indexing
                        } else {
                            median = (resultSorted[(resultSorted.size)/2] +
                                        resultSorted[(resultSorted.size-2)/2]) / 2 ;
                        }
                        writeln(testName, " ",atts," ", blockSize, attsSuffix," Median: ", median, " ", units);
                    }
                }
            }
        }
    }
}

module ResultDatabaseTest {
    use ResultDB;
    use IO.FormattedIO;
    proc testOutput(){
        var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
        var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
        // var randStreamSeeded = new RandomStream(real, 1032435);

        flopsDB.printDatabaseStats();

        const passes = 10;
        var blockSizes = [64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384];
        for pass in 0..#passes{
            for blkSize in blockSizes {
                // Running a test would happen in a block like this for Triad
                // try! flopsDB.addToDatabase("%{#####}".format(blkSize), randStreamSeeded.getNext()*100);
                try! flopsDB.addToDatabase("%{#####}".format(blkSize), 12.38);
                // try! bdwthDB.addToDatabase("%{#####}".format(blkSize), randStreamSeeded.getNext()*100);
                try! bdwthDB.addToDatabase("%{#####}".format(blkSize), 79.55565);
            }
        }

        flopsDB.printDatabaseStats();
        bdwthDB.printDatabaseStats();
    }
    // testOutput();
}
