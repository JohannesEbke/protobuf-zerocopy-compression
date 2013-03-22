#ifndef _PROTOBUF_ZEROCOPY_COMPRESSION_COMPRESSED_STREAM_H_
#define _PROTOBUF_ZEROCOPY_COMPRESSION_COMPRESSED_STREAM_H_

#include <google/protobuf/io/zero_copy_stream.h>

namespace zerocc {

/// Base class for all compressed output streams with additional methods
class AbstractCompressedOutputStream : public google::protobuf::io::ZeroCopyOutputStream {
public:
    /// Make sure that all data is compressed and written
    virtual bool Flush() = 0;
    virtual bool Close() = 0;
};

/// Base class for all compressed input streams with an ExpectAtEnd method.
class AbstractCompressedInputStream : public google::protobuf::io::ZeroCopyInputStream {
public:
    /// ExpectAtEnd returns true if there is no more compressed data to process
    virtual bool ExpectAtEnd() = 0;
};

typedef enum {
    ZLIB = 1,
    GZIP,
    SNAPPY,
    LZ4
} CompressionType;

/// Get a pointer to a compressed output stream given an underlying ZeroCopyOutputStream.
/// Specify any of the above compression types, and a compression level (for use in ZLIB).
AbstractCompressedOutputStream * get_compressed_output_stream(google::protobuf::io::ZeroCopyOutputStream * ostream, CompressionType t, int level=1);

/// Get a pointer to a compressed input stream given an underlying ZeroCopyInputStream.
/// Specify any of the above compression types.
AbstractCompressedInputStream * get_compressed_input_stream(google::protobuf::io::ZeroCopyInputStream * istream, CompressionType t);

}; // namespace zerocc

#endif
