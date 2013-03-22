protobuf-zerocopy-compression
=============================

Some Compression streams (gzip, snappy, lz4) implementing the ZeroCopy Interface from Google(TM) protobuf 2.4.1

Quickstart
----------

The main interface is defined and documented in https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/compressed_stream.h. 

An example program is https://github.com/JohannesEbke/protobuf-zerocopy-compression/blob/master/test.cc

A quick look:
```
#include <compressed_stream.h>
...
AbstractCompressedInputStream * in = get_compressed_input_stream(my_infile_stream);
...
AbstractCompressedOutputStream * out = get_compressed_output_stream(my_outfile_stream);
```

To compile the static library and the test program, run

```
./waf configure
./waf
```

Have fun!


