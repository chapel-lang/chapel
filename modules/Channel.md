## Channel Module

A channel is a FIFO data structure used to communicate between different Chapel
tasks. You can learn more about how go-channels work by following this
[link](https://gobyexample.com/channels)
Since the operations on the channel are blocking by default, it enables to send
values from one task to another in a synchronized manner.
Here is a simple example in Go.

```
func main() {
	// make a channel for string values
	messages := make(chan string)

	// new task to send a value to the channel
	// it will wait until other task receives
	go func() { messages <- "ping" }()

	// receive the sent value
	msg := <-messages
	fmt.Println(msg)
}
```

The module will have a generic `Channel` class to store the channel buffer and
support operations on them.

* Here are some functions that the module should have

	* `init(type eltType, size : int)` : Specify the buffer data type and size
	for creating a Channel.

	* `send(value : eltType)` : This function waits for a space in the buffer
	and	sends the `value` to the channel.

	* `recv() : eltType` : This function waits until the buffer is empty and
	returns the value sent to the channel.

	* `close()` : This function is used to close the channel such that no more
	values will be sent to it.

* `iter these()` : An iterator to receive the values sent to the channel until
it is closed.

* `select` statements : This statement lets you wait for multiple channel
operations and executes if anyone of them is ready for communication.

For e.g.
```
// Select syntax in Go to wait on any one of the two channels for receiving
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

* What should be the name of the module ? Some names in other languages
	* `chan` in Golang
	* `crossbeam` in Rust

* What should be the syntax of `select` statements in Chapel? Should we use
Chapel's `select` statement for that? Some semantic differences between
`select` in Chapel and Channels.

	* Chapel's select is a conditional statement which is used to evaluate
	cases against one statement specified after `select` keyword whereas
	`select` in Channels work with multiple Channel operations.

	* `select` for Channels can be blocking or non-blocking while `select`
	in Chapel is always non-blocking.

* Should operators be used for `send` and `recv` routines ?
