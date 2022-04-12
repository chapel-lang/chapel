/* Aggregator */

config const maxBatchedItemsPerLocale = 1024; // size of each aggregation buffer

//
// An instance of Aggregator should be used on the locale where it was created,
// with all fields holding local values. Not par-safe.
//
// Currently it aggregates only indices, for simplicity.
// It can be generalized to aggregate arbitrary-typed inputs such that:
// * the index can be extracted from each input element using a separate FCF,
//   to be provided as an argument to the aggregator;
// * such entire input element, not just the index, is passed
//   to the `updater` FCF.
//
// The aggregator is designed to be usable with any "client" data structure
// that implements the API indicated in the code using "API" comments.
//
class Aggregator {
  const client;    // the data structure to aggregate for
  const updater;   // must provide updater.this(idx, ref element):void

  var batches: [client.targetLocales.domain]    // API: client.targetLocales
                  _batchedItemsPerLocale(client.keyType);

  /* aggregate the `key` */
  proc add(in key: client.keyType) {            // API: client.keyType
    const locIdx = client.localeIdxForKey(key); // API: client.localeIdxForKey()
    ref   batch  = batches[locIdx];
    const full   = batch.add(key);
    if full {
      on client.targetLocales[locIdx] do
        applyUpdatesHere(batch);
      batch.clear();
    }
  }

  proc deinit() {
    sendAndApplyFinalUpdates();
  }

  proc sendAndApplyFinalUpdates() {
    coforall (loc, batch) in zip(client.targetLocales, batches) do
      on loc do
        applyUpdatesHere(batch);

    forall batch in batches do
      batch.clear();
  }

  proc applyUpdatesHere(ref batch) {
    // API: client.getPrivatizedThis()
    const localClient = client.getPrivatizedThis();
    var   localBatch  = batch.buffer[0..#batch.size]; // bulk copy
    // API: client.applyAggregatedUpdates
    /* enact the updates here */
    localClient.applyAggregatedUpdates(localBatch, updater);
  }
} // class Aggregator

record _batchedItemsPerLocale {  // private
  type itemType; // the type of things are we aggregating
  var size = 0;
  var buffer: [0..#maxBatchedItemsPerLocale] itemType;

  // returns true if the buffer ends full
  proc add(in item: itemType): bool {
    buffer[size] = item;
    size += 1;
    return size >= maxBatchedItemsPerLocale;
  }

  proc clear() {
    size = 0;
    // todo: how to clear up individual items?
    // they should be moved to the client data structure
    // whether the destination is on the current or remote locale
  }
}
