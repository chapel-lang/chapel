.. _mason-packages-index:

Mason Packages (WIP)
====================

Overview
--------

This guide on Mason Packages contains everything that you need to
know to hit the ground developing in Chapel using mason, whether
that be to create a library to contribute to the community, use an
existing package, or just to help with the process of building your
Chapel programs.

.. note::

   **Pardon Our Dust**

   This mason guide is a work-in-progress as mason development itself
   is still a work-in-progress.  If you're not already using the `main
   branch <https://chapel-lang.org/docs/main/mason-packages/>`_ of the
   documentation (which tracks real-time GitHub development), you may
   find that it contains a more complete set of sections.  We welcome
   feedback and requests on the guide as we go.


The Chapel Users Guide is divided into four main sections:


Getting Started with Mason
--------------------------

In this section, you will find information on installing mason
and a basic HelloWorld example to get you started using mason
packages in your own code.

.. toctree::
   :maxdepth: 1
   :glob:

   start/installation
   start/helloworld
   start/whymason


.. _index-mason-guide:

Mason Guide
-----------

This section aims to explain why a user would want to use mason,
point out what types of users would benefit from using mason, outline
some typical use-cases of mason, and introduce some of the features
that make using mason worthwhile.

.. toctree::
   :maxdepth: 1
   :glob:

   guide/basicusage
   guide/buildingandrunning
   guide/buildinglargerpackages
   guide/runtimearguments
   guide/testing
   guide/examples
   guide/documentation
   guide/chapeldependencies
   guide/gitdependencies
   guide/externaldependencies
   guide/masonregistry
   guide/submitting
   guide/manifestfile
   guide/environmentvariables
   guide/namespacing
