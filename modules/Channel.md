## Channel Module

A channel is a FIFO data structure used to communicate between different Chapel
tasks. You can learn more about how go-channels work by following this
[link](https://gobyexample.com/channels)
Since the operations on the channel are blocking by default, it enables to send
values from one task to another in a synchronized manner.

The module will have a `Channel` class to store the channel buffer and support
operations on them.

* Here are some functions that the module should have

	*  `send(value : eltType)` : This function waits for a space in the buffer
	and	sends the `value` to the channel.

	*  `recv() : eltType` : This function waits until the buffer is empty and
	returns the value sent to the channel.

	*  `close()` : This function is used to close the channel such that no more
	values will be sent to it.

* `iter these()` : An iterator to receive the values sent to the channel until
it is closed.

* `select` statements : This statement lets you wait for multiple channel
operations and executes if anyone of them is ready for communication.

For e.g.
```
// Select syntax in Go to wait on any one of the two channels is ready
// If every channel is blocked, the default case is executed.
select {
	case msg1 := <-chan1 :
		fmt.Println("Received", msg1)
	case msg2 := <-chan2 :
		fmt.Println("Received", msg2)
	default :
		fmt.Println("None")
}
```

Some related questions -

* Should this module be standard or package ?

* What should be the name of the module ?
	* proposal : `Channels`

* Should the Channel select be used with the Chapel
[select](https://chapel-lang.org/docs/language/spec/statements.html#the-select-statement)
syntax or something else?

* Should operators be used for `send` and `recv` routines ?
