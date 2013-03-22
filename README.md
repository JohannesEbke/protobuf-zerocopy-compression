protobuf-zerocopy-compression
=============================

This library implements some basic compression streams implementing the [ZeroCopy Interface](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream) from [Google(TM) protobuf 2.4.1](http://code.google.com/p/protobuf/). Currently, gzip, zlib, snappy (external) and lz4 (included) are supported, but any blockwise compression algorithm could be plugged in with little effort.

Quickstart:

The main interface is defined and documented in [compressed_stream.h](https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/compressed_stream.h).

An example program is [test.cc](https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/test.cc)

A quick look:
```c++
#include <compressed_stream.h>
...
AbstractCompressedInputStream * in = get_compressed_input_stream(my_infile_stream);
...
AbstractCompressedOutputStream * out = get_compressed_output_stream(my_outfile_stream);
```

To compile the static library and the test program, run

```bash
./waf configure
./waf
```

Have fun!


