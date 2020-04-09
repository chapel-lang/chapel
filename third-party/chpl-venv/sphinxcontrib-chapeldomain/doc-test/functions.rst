Test the functions...
=====================

Chapel Functions
----------------

.. default-domain:: chpl

.. FIXME: make sure functions without parens display correctly
.. function:: noParens

    A function without parenthesis. What is the world coming to?
    :chpl:proc:`sendIt`. See also :chpl:proc:`noParensRet`.

    :returns: something really, really interesting... jk

.. function:: noParensRet: int

    A function without parenthesis with a return type. See also
    :chpl:proc:`noParens`...

    :rtype: int
    :returns: integer value with invoking function (e.g. just call
              ``noParensRet``)

.. function:: send()

    Send something... blah blah :chpl:proc:`messageId`. blah blah
    :chpl:proc:`noParens`

.. function:: sendIt()

    Send it. See :chpl:proc:`send` and :chpl:proc:`sendItAgain`.

    :returns: it
    :rtype: Message

.. function:: sendItAgain(): Message

    Send it. See :chpl:proc:`sendMessage`...

    :returns: it
    :rtype: Message

.. function:: inline proc messageId(msg)

    Returns the message id.

    :param Message msg: the message
    :returns: the message id
    :rtype: int

.. function:: takeAType(type someType)

    Takes a generic type of some sort... see also :chpl:proc:`returnRef`.

    :arg type someType: the generic type
    :returns: new instance of someType

.. function:: returnRef() ref

    Returns reference to some value. See also :chpl:proc:`takeAType`.

    :returns: reference to known value
    :rtype: MyType

.. function:: sendMessage(sender, recipient, message_body, [priority=1])

    Send a message to a recipient. ... :chpl:proc:`sendMessageFullyTyped` ...

    :arg string sender: The person sending the message
    :arg string recipient: The recipient of the message
    :arg string message_body: The body of the message
    :param priority: The priority of the message, can be a number 1-5
    :type priority: integer or None
    :return: the message id
    :rtype: int

.. function:: sendMessageFullyTyped(sender: string, recipient: string, message_body: string, [priority: int=1]): int

    Send a message to a recipient... see also :chpl:proc:`sendMessage`

    :arg string sender: The person sending the message
    :arg string recipient: The recipient of the message
    :arg string message_body: The body of the message
    :param priority: The priority of the message, can be a number 1-5
    :type priority: integer or None
    :return: the message id
    :rtype: int

.. iterfunction:: fib(n)

    Iterate through first ``n`` Fibonacci numbers. Can xref me with
    ``:chpl:iter:``? :chpl:iter:`fib` ? How about ``:chpl:proc:``?
    :chpl:proc:`fib` ? And ``:chpl:proc:``? :chpl:proc:`fib` ?

    :arg int n: Number of Fibonacci numbers to return.
    :ytype: int
    :yields: Fibonacci numbers, first through nth.

.. function:: proc foo(x: int(?bits))

    :type x: int(?bits)
    :type x: int(32)
    :arg x: x is good
    :type x: int
    :type x: int(64)

.. function:: proc rcRemote(replicatedVar: [?D] ?MYTYPE, remoteLoc: locale) ref: MYTYPE

    Remote stuff... :proc:`rcLocal`

.. function:: proc rcLocal(replicatedVar: [?D] ?MYTYPE) ref: MYTYPE

    Local stuff... :proc:`rcRemote`

.. function:: proc specialArg(const ref x: int)

.. function:: proc specialReturn() const ref

.. function:: proc constRefArgAndReturn(const ref x: int) const ref

.. function:: proc <(x, y)

.. function:: proc throwsAnError() throws

    :throw FileNotFoundError: looks at a file that doesn't exist

.. function:: proc throwsVariousErrors(x: bool) throws

    :arg x: determines whether to check a directory or a file
    :type x: bool
    :throw IsADirectoryError: makes a directory and does something dumb
    :throw NotADirectoryError: makes a file that it treats like a directory

.. function:: proc throwsOrReturns(): string throws

    :return: a message for you!
    :rtype: string
    :throw PermissionError: when the message is not for you


Other stuff...
--------------

Do these xref work?

* :chpl:func:`~Containers.Vector.push`
* :chpl:proc:`~Containers.Vector.push`
* :chpl:iter:`~Containers.Vector.push`
* :chpl:meth:`~Containers.Vector.push`

Python functions
----------------

.. py:function:: send()

    Send something...

.. py:function:: send_it()

    Send it.

    :returns: it
    :rtype: Message

.. py:function:: send_message(sender, recipient, message_body, [priority=1])

    Send a message to a recipient

    :param str sender: The person sending the message
    :param str recipient: The recipient of the message
    :param str message_body: The body of the message
    :param priority: The priority of the message, can be a number 1-5
    :type priority: integer or None
    :return: the message id
    :rtype: int
    :raises ValueError: if the message_body exceeds 160 characters
    :raises TypeError: if the message_body is not a basestring
