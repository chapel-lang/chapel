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

   guide/whymason
ASDASBNOGEBIUEWGIOUEWHIO UPDATE BASED OFF RUST DOCS

(more to come...)


.. _index-mason-reference:

Mason Reference
---------------

This section outlines some of the semantics behind things such as
mason TOML files, mason lock files, and other mason features.

.. toctree::
   :maxdepth: 1
   :glob:

   datapar/forall
   datapar/promotion

(more to come...)


Mason Commands
--------------

These are Chapel's features for describing how data and tasks should
be mapped to the target architecture for the purposes of performance
and scalability.

.. toctree::
   :maxdepth: 1
   :glob:

   locality/localesInChapel
   locality/compilingAndExecutingMultiLocalePrograms
   locality/localeTypeAndVariables
   locality/onClauses

(more to come...)

