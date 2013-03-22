protobuf-zerocopy-compression
=============================

This C++ library implements some basic compressed input/output streams implementing the [ZeroCopy Interface](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream) from [Google(TM) protobuf 2.4.1](http://code.google.com/p/protobuf/). Currently, [gzip, zlib](zlib.net), [snappy](http://code.google.com/p/snappy/) and [lz4](http://code.google.com/p/lz4/) (included) are supported, but any blockwise compression algorithm could be plugged in with little effort.

Quickstart
----------

The main interface is defined and documented in [compressed_stream.h](https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/compressed_stream.h).

An example program is [test.cc](https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/test.cc)

A quick look:
```c++
#include <compressed_stream.h>
...
zerocc::AbstractCompressedInputStream * in = get_compressed_input_stream(&my_infile_stream);
...
zerocc::AbstractCompressedOutputStream * out = get_compressed_output_stream(&my_outfile_stream, zerocc::ZLIB, 4);
```

To compile the static library and the test program, just run

```
./waf go
```

Have fun!


