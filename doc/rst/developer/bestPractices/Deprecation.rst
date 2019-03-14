Chapel's Deprecation Policy
===========================

Chapel's general policy for deprecation is to:

- Provide a compiler warning (or error) when a deprecated feature is used
- Continue to document and support the deprecated feature for at least 1
  release cycle
  - The documentation should contain a pointer to the preferred feature
  - This holds true for modules as well

The deprecation message is typically implemented with the following format:

..code-block:: chapel

    CompilerWarning("foo is deprecated - please use bar")

