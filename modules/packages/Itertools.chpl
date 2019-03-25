
/*
    This module serves as a toolkit of common serial and parallel
    iterators. This is currently being worked upon, and contains
    only the ``repeat`` iterator tool as of now.
*/

module Itertools {

    /*
        Returns an object over and over again, a specified
        number of times.


        :arg arg: The object to be returned
        :type arg: `?`

        :arg times: The number of times to return
        :type times: `int`

        :yields: Object in the range ``1..times``


        If the argument ``times`` has the value 0, it will return
        the object an infinite number of times.

        This iterator can be called in serial and zippered contexts.
    */

    //  Serial iterator

    iter repeat (arg, times = 0) {
        if times == 0 then
            for count in 1.. do yield arg;
        else
            for count in 1..#times do yield arg;     
    }

    //  Standalone parallel iterator

    pragma "no doc"
    iter repeat (param tag: iterKind, arg, times = 0)
            where tag == iterKind.standalone {
        if times == 0 then
            halt("Infinite iteration not supported for parallel loops.");
        else
            coforall count in 1..#times do yield arg;
    }

    //  Procedure to compute the chunks to be iterated through

    pragma "no doc"
    proc computeChunk(r: range, myChunk, numChunks)
            where r.stridable == false {
        const numElems = r.length;
        const elemsPerChunk = numElems / numChunks;
        const mylow = r.low + (elemsPerChunk * myChunk);

        if (myChunk != numChunks - 1) then
            return mylow..#elemsPerChunk;
        else
            return mylow..r.high;
    }

    /*
        Used to set the maximum number of tasks supported by locale.
        See `here <https://chapel-lang.org/docs/builtins/ChapelLocale.html?highlight=here#ChapelLocale.here>`_ and
        `maxTaskPar <https://chapel-lang.org/docs/builtins/ChapelLocale.html?highlight=maxTaskPar#ChapelLocale.locale.maxTaskPar>`_
        for more information.
    */
    
    config const numTasks = here.maxTaskPar;

    //  Parallel iterator - Leader

    pragma "no doc"
    iter repeat (param tag: iterKind, arg, times = 0)
            where tag == iterKind.leader {
        if times == 0 then
            halt("Infinite iteration not supported for parallel loops.");
        else
            coforall tid in 0..#numTasks {
                const working_iters = computeChunk(0..#times, tid, numTasks);
                yield(working_iters,);
            }
    }

    //  Parallel iterator - Follower
    
    pragma "no doc"
    iter repeat (param tag: iterKind, arg, times = 0, followThis)
            where tag == iterKind.follower && followThis.size == 1 {
        const working_iters = followThis(1);

        for idx in working_iters do yield arg;
    }
} // end module
