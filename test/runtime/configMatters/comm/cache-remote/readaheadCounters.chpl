/*
    This tests the CommDiagnostics counters for following remote cache readahead counters:

    cache_num_page_readaheads: how many cache pages were read ahead
    cache_readahead_unused: how many cache pages that were read ahead were evicted before being accessed once
    cache_readahead_waited: how many cache pages that were read ahead had not arrived by the time we tried to access it.

    A readahead is performed automatically by the remote cache when sequential accesses to remote memory
    is detected. For example, if a GET is performed adjacent to the previous GET, the readahead logic
    will do fetch the rest of the data in the given page.

    Given the nature of this test, it must be executed with at least 2 locales.
*/

use CommDiagnostics;
use IO;
use CTypes;

// Helper function we use in this test to invalidate pages that were read ahead
// into the remote cache
extern proc chpl_cache_invalidate(node:c_int, raddr:c_ptr(void),
                                  size: c_size_t);

// Constants for the tests
config const numElems = 4096;   // number of elements in the test array
const elemSize = 8;             // always assuming 8-byte elements in the array
const pageSize = 1024;          // cache page size assumed to be 1024 bytes
const elemsPerPage = pageSize/elemSize; // how many elements in the array would fall into
                                        // a cache page

// Main procedure called to perform the test. Different IDs (1, 2, etc.) are
// passed in to run a specific test case
proc testCounters(id: int)
{
    // The array that is allocated on locale 0. We'll use this as the target of
    // the readaheads.
    var A : [0..#numElems] int;

    //################################################################################################
    //
    // Test 1: Readaheads are triggered by sequential accesses (i.e., a GET that is to adjacent
    // memory of a previous GET). This test performs non-adjacent accesses so that readaheads are NOT
    // triggered. Each access we do is separated by a cache page.
    //
    //################################################################################################
    if id == 1 {
        A = 1;
        const numReadaheads = (numElems*elemSize) / pageSize;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            var sum = 0;
            for i in 0..#numReadaheads {
                const idx = i*elemsPerPage;
                sum += A[idx];
            }
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_page_readaheads = res[1].cache_num_page_readaheads;
        var cache_readahead_unused = res[1].cache_readahead_unused;
        var cache_readahead_waited = res[1].cache_readahead_waited;
        assert(cache_num_page_readaheads == 0);
        assert(cache_readahead_unused == 0);
        assert(cache_readahead_waited == 0);
        writef("\tcache_num_page_readaheads: %i\n", cache_num_page_readaheads);
        writef("\tcache_readahead_unused: %i\n", cache_readahead_unused);
        writef("\tcache_readahead_waited: %i\n", cache_readahead_waited);
    } /* end of test 1 */

    //################################################################################################
    //
    // Test 2: Opposite of test 1, where we do nothing but adjacent accesses. The only thing we can
    // assert to be true is that we will have 0 unused readaheads and the number of readaheads is
    // non-zero.
    //
    // The number of readaheads and how many we had to wait on is not something that can be easily 
    // determined based on the number of accesses we make. It seems to be deterministic, in that the 
    // code below triggers 24 readaheads and 16 waited-on readaheads when the size of A is 4096 and
    // the --fast flag is NOT used when compiling. But that could be different on some other machine 
    // with a different interconnect (at least the number of waited-on readaheads would be affected).
    //
    //################################################################################################   
    if id == 2 {
        A = 1;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            var sum = 0;
            for i in 0..#numElems {
                sum += A[i];
            }
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_page_readaheads = res[1].cache_num_page_readaheads;
        var cache_readahead_unused = res[1].cache_readahead_unused;
        assert(cache_num_page_readaheads > 0);
        assert(cache_readahead_unused == 0);
        writef("\tcache_num_page_readaheads: greater than zero\n");
        writef("\tcache_readahead_unused: %i\n", cache_readahead_unused);
    } /* end of test 2 */
}

proc main()
{
    writef("Running test 1\n");
    testCounters(1);

    writef("Running test 2\n");
    testCounters(2);
}
