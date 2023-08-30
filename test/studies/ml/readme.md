# ML in Chapel

### 2023 Summer internship project of  Iain Moncrief

Here are descriptions of each of the performance tests:
- `performance/mnist_batch.chpl` is a moderately sized CNN whith a high batch size, as to utilize as much parallelism as possible.
- `performance/mnist_cnn_no_batch.chpl` is a two layer CNN that is trained with batch sizes of 1.
- `performance/mnist_cnn_deep.chpl` has three convolution layers and is a representative for *deeper* convolutional neural networks. 
- `performance/mnist_cnn_no_stride.chpl` is a two layer CNN that uses a stride of 1 on the first convolution layer. 
- `performance/mnist_cnn_stride.chpl` is the same as the no-stride test, except that has a stride of 2 on the first layer. This difference downsamples the input by a factor of 2, at the first layer. 
- `performance/mnist_cnn_with_dense.chpl` is a two layer CNN with an additional dense layer of 200 neurons. 
- `performance/xor_perceptron.chpl` is not a CNN, but just a regular MLP that learns the 2-ary XOR function. 