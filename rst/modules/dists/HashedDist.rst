.. default-domain:: chpl

.. module:: HashedDist

HashedDist
==========
**Usage**

.. code-block:: chapel

   use HashedDist;

.. class:: Hashed

   
   
   The Hashed distribution maps an associative domain and its array to a set of
   target locales. Each index (or key) in the domain is mapped to a locale based
   upon its value. When constructing a Hashed distribution, you can optionally
   provide a `mapper` function or function object to compute the destination locale
   for each index.
   
   The `mapper` provided can be a class, record, or first class function.  When
   called, it will be passed the index and the targetLocales array that the Hashed
   distribution was initialized with. For example, the following record
   declares a custom mapper:
   
   .. code-block:: chapel
   
     record CustomMapper {
       proc this(ind:string, targetLocs: [?D] locale) : D.idxType {
         const numlocs = targetLocs.domain.size;
         // use the first digit of the string to choose the destination locale
         var byte: int = ind.byte(1);
         return byte % numlocs;
       }
     }
   
   If a custom mapper is not provided, a default mapper will be used. The default
   mapper computes the target locale based upon a hash of the index.
   
   .. note::
   
     Hashed is not yet interface stable.
   
   **Example**
   
   .. code-block:: chapel
   
     var D: domain(string) dmapped Hashed(idxType=string);
     // Now D is a distributed associative domain (set) of strings
     D += "one";
     D += "two";
   
     var A: [D] int;
     // Now A is a distributed associative array (map) from string to int
     forall a in A do
       a = a.locale.id;
   
     forall (key, value) in zip(D, A) {
       writeln(key, " -> ", value);
     }
   
   
   **Initializer Arguments**
   
   The `Hashed` domain map initializer is defined as follows:
   
   .. code-block:: chapel
   
     proc init(type idxType,
               mapper:?t = new DefaultMapper(),
               targetLocales: [] locale = Locales)
   
    


