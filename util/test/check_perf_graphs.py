import os
import fnmatch


def get_graph_files(test_dir):
    return list([f for f in os.listdir(test_dir) if f.endswith("GRAPHFILES")])


def get_perf_labels(test_dir):
    graphs_files = get_graph_files(test_dir)
    perf_labels = ["perf"]
    for graphs_file in graphs_files:
        prefix = graphs_file.removesuffix("GRAPHFILES")
        prefix = prefix.lower()
        if prefix:
            perf_labels.append(prefix)
    return perf_labels


def check_for_duplicates(
    logger,
    on_error,
    test_dir,
    perflabels,
    check_duplicate_tests,
    check_missing_graphs,
):
    # check for .dat duplicates
    if check_duplicate_tests:
        logger.write("[Checking for duplicate performance data filenames]")

        dat_files = []
        error = False
        for root, dirnames, filenames in os.walk(test_dir):
            if not isinstance(perflabels, list):
                perflabels = [perflabels]

            for perflabel in perflabels:
                for filename in fnmatch.filter(filenames, "*." + perflabel):
                    if filename in dat_files:  # duplicate
                        logger.write(
                            "[Error: Duplicate performance data filenames"
                            " ({0})".format(filename)
                        )
                        error = True
                    else:
                        dat_files.append(filename)

        if error:
            on_error()

    # check for .graph files, and GRAPHFILES
    if check_missing_graphs:
        logger.write(
            "[Checking for duplicate .graph files and that all .graph"
            " files appear in {0}/*GRAPHFILES]".format(test_dir)
        )

        # find GRAPHFILES
        graph_files = get_graph_files(test_dir)

        # read .graph files from GRAPHFILES
        GRAPHFILES_graph_files = []
        for file in graph_files:
            with open(os.path.join(test_dir, file), "r") as f:
                for line in f:
                    if line == "":
                        continue
                    if line.strip() == "" or line.strip()[0] == "#":
                        continue
                    GRAPHFILES_graph_files.append(line.rstrip())

        # get absolute paths of actual .graph files
        actual_graph_files = []
        for root, dirnames, filenames in os.walk(test_dir):
            for filename in fnmatch.filter(filenames, "*.graph"):
                actual_graph_files.append(
                    os.path.relpath(os.path.join(root, filename), test_dir)
                )

        # check that actual .graph files are listed in GRAPHFILES
        for g in actual_graph_files:
            filename = g
            if filename not in GRAPHFILES_graph_files:
                logger.write(
                    "[Warning: {0} is missing from GRAPHFILES]".format(filename)
                )

        # check that all .graph files in GRAPHFILES actually exist
        for g in GRAPHFILES_graph_files:
            filename = g
            if filename not in actual_graph_files:
                logger.write(
                    "[Warning: {0} listed in GRAPHFILES does not "
                    "exist]".format(filename)
                )

        # check for duplicates
        graph_set = {}
        for g in actual_graph_files:
            filename = os.path.basename(g).lower()
            if filename in graph_set:
                logger.write(
                    "[Warning: graph files must have unique, case "
                    "insensitive names: {0} and {1} do not]".format(
                        g, graph_set[filename]
                    )
                )
            else:
                graph_set[filename] = g


def main():

    class Logger:
        def write(self, msg):
            print(msg)

    def on_error():
        exit(1)

    logger = Logger()

    test_dir = os.path.abspath(os.path.dirname(__file__))
    perflabels = get_perf_labels(test_dir)
    check_for_duplicates(logger, on_error, test_dir, perflabels, True, True)


if __name__ == "__main__":
    main()
