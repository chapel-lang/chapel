.. _readme-knl:

=======================
Intel "Knights Landing"
=======================

The following information is assembled to help Chapel users get up and
running on Intel Xeon Phi, Knights Landing (KNL).

The initial implementation has only been tested on Cray machines where
the KNL chip is the host processor. While we have not explicitly tested
on other platforms, or with KNL as a Coprocessor, we don't know of any
reason an advanced user couldn't run on such a platform.

---------------
Getting started
---------------

By and large running on KNL on a Cray XC will be the same as running on
a Xeon based Cray XC. See :ref:`readme-cray` for more info. 

In order to take better advantage of the AVX-512 micro-architecture
you'll want to have a KNL targeting module loaded. For example::
    
  module load craype-mic-knl

You'll also want to ensure you have a new enough target compiler loaded
that is KNL/AVX-512 ready. We recommend using at least gcc 5.3, cce 8.5,
or intel 16.

Currently we don't provide any mechanism to manage the MCDRAM (High
Bandwidth Memory/HBM.) As a result, we recommend running with HBM in
cache-only mode. We intend to add HBM support (through Locale Models)
in future releases.

-----------------
Future Directions
-----------------

As mentioned above, we plan to add support for targeting the HBM through
our Locale Model interface.

We also plan to explore which clustering mode(s) tend to work best for a
typical Chapel program.
