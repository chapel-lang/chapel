.. default-domain:: chpl

.. module:: RandomSupport
   :synopsis: Seed generation for pseudorandom number generation

RandomSupport
=============
**Usage**

.. code-block:: chapel

   use Random.RandomSupport;


Seed generation for pseudorandom number generation


.. note::

  For many of the functions in :mod:`Random`, if no seed is provided by the
  user, one is chosen based on the current time in microseconds, allowing
  for some degree of variability in seed selection.  The intent of
  :record:`SeedGenerator` is to provide a menu of other options for
  initializing the random stream seed, but few options are implemented
  at present. In particular, it would be useful to allow multiple tasks to
  create different seeds at the same time, or to allow different
  functions running at the same time to produce different seeds.



.. record:: SeedGenerator

   
   Provides methods to help generate seeds when the user doesn't want
   to create one.  It currently supports two type methods. Both start
   with the current time.
   


   .. method:: proc type currentTime: int(64)

      
      Generate a seed based on the current time in microseconds as
      reported by :proc:`Time.getCurrentTime`. This seed is not
      suitable for the NPB RNG since that requires an odd seed.
      

   .. method:: proc type oddCurrentTime: int(64)

      
      Generate an odd seed based on the current time in microseconds as
      reported by :proc:`Time.getCurrentTime`. This seed is suitable
      for the NPB RNG.
      

