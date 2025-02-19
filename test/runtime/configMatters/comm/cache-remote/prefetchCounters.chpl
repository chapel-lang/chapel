/*
    This tests the CommDiagnostics counters for following remote cache prefetch counters:

    cache_num_prefetches: how many cache pages were prefetched via chpl_comm_remote_prefetch 
    cache_prefetch_unused: how many cache pages that were prefetched were evicted before being accessed once
    cache_prefetch_waited: how many cache pages that were prefetches had not arrived by the time we tried to access it.

    Given the nature of this test, it must be executed with at least 2 locales.
*/

use CommDiagnostics;
use IO;
use CTypes;

// This is a wrapper around the function that we currently use to
// issue a prefetch. Prefetches via this function are what the CommDiagnostics
// counters target.
inline proc prefetch(ref x, len:int) {
  __primitive("chpl_comm_remote_prefetch", x.locale.id, x, len);
}

// Helper function we use in this test to invalidate pages we prefetched
// into the remote cache
pragma "insert line file info"
extern proc chpl_cache_invalidate(node:c_int, raddr:c_ptr(void),
                                  size: c_size_t);

// Constants for the tests
config const numElems = 4096;   // number of elements in the test array
const numBusyElems = 8*numElems;// number of elements in the busy work array
const elemSize = 8;             // always assuming 8-byte elements in the array
const pageSize = 1024;          // cache page size assumed to be 1024 bytes
const elemsPerPage = pageSize/elemSize; // how many elements in the array would
                                        // fall into a cache page

// Main procedure called to perform the test. Different IDs (1, 2, etc.) are
// passed in to run a specific test case
proc testCounters(id: int)
{
    // The array that is allocated on locale 0. We'll use this as the target of
    // the prefetches that are issued from locale 1.
    var A : [0..#numElems] int;

    // Another array used for remote accesses in some tests. Their purpose is to
    // do "busy work" to ensure the prefetches we issued are complete. This is
    // done in the cases where we want to ensure that there are no waited-on
    // prefetches.
    var B : [0..#numBusyElems] int;

    // temp we use in some cases
    var temp = 0;

    //#########################################################################
    //
    // Test 1: Perform prefetches and do not access anything. We should see 0
    // prefetches that were waited on and all prefetches should be marked as
    // unused. To get the unused count to work "correctly", we need to evict the
    // cache pages that were prefetched (a current issue/feature is that when
    // the commDiagnostics stop, we don't account for anything left in the
    // remote cache).  Furthermore, we insert a loop that does some "busy work"
    // so that the prefetches complete. That ensures that the waited counter is
    // 0.
    //
    //#########################################################################
    if id == 1 {
        // ensure prefetches are 1024 bytes apart (i.e., a cache page).
        const numPrefetches = (numElems*elemSize) / pageSize;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                prefetch(A[idx], 1);
            }
            // kill some time so the prefetches above will complete.
            // This ensures that the waited counter will be 0.
            for i in 0..#numBusyElems {
                temp += B[i];
            }
            // force evictions of the pages we prefetched. Since we didn't
            // access any, this ensures the unused counter is equal to the
            // number of prefetches we did earlier.
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        //assert(cache_num_prefetches == numPrefetches);
        //assert(cache_prefetch_unused == numPrefetches);
        //assert(cache_prefetch_waited == 0);
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);
    } /* end of test 1 */

    //#########################################################################
    //
    // Test 2: Perform same number of prefetches as test 1, but access the data
    // associated with "even" number prefetches. This tests that the unused
    // counter is working correctly. We should see that half of the prefetches
    // we issued went unused before being evicted. Again, we should see 0
    // prefetches that were waited on.
    //
    //#########################################################################
    if id == 2 {
        A = 1;
        var numAccessed = 0;
        const numPrefetches = (numElems*elemSize) / pageSize;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                prefetch(A[idx], 1);
            }
            // kill time so prefetches complete
            for i in 0..#numBusyElems {
                temp += B[i];
            }
            // access "even" prefetches
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                if i % 2 == 0 {
                    temp += A[idx];
                    numAccessed += 1;
                }
            }
            // force evictions of the pages we prefetched
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }   
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        //assert(cache_num_prefetches == numPrefetches);
        //assert(cache_prefetch_unused == numPrefetches-numAccessed);
        //assert(cache_prefetch_waited == 0);
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);          
    } /* end of test 2 */

    //#########################################################################
    //
    // Test 3: Attempt to access the data prefetched immediately after it was
    // prefetched. This should result in all prefetches being counted as
    // "waited". We should see 0 unused prefetches, since we access everything
    // we prefetched.
    //
    //#########################################################################
    if id == 3 { 
        const numPrefetches = (numElems*elemSize) / pageSize;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                prefetch(A[idx], 1);
                temp += A[idx];
            }
            // force evictions of the pages
            for i in 0..#numPrefetches {
                const idx = i*elemsPerPage;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        //assert(cache_num_prefetches == numPrefetches);
        //assert(cache_prefetch_unused == 0);
        //assert(cache_prefetch_waited == numPrefetches);
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);
    } /* end of test 3 */  
}

proc main()
{
    writef("Running test 1\n");
    testCounters(1);

    writef("Running test 2\n");
    testCounters(2);

    writef("Running test 3\n");
    testCounters(3);
    
}
