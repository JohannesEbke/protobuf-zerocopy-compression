// Author: peter.waller@gmail.com (Peter Waller)
// Author: johannes@ebke.org (Johannes Ebke)

#include <iostream>
#include <cmath>

#include <assert.h>

#include <google/protobuf/stubs/common.h>

#include <google/protobuf/io/coded_stream.h>
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;

#include "block_wrappers.h"

#if HAVE_SNAPPY
#include <snappy.h>
#endif
#include "lz4.h"

namespace zerocc {

const size_t BLOCKSIZE = 64 * 1024;

BlockCompressionInputStream::BlockCompressionInputStream(
    ZeroCopyInputStream* sub_stream)
    : _output_buffer(NULL), _output_buffer_size(0), _sub_stream(NULL), _backed_up_bytes(0), 
      _byte_count(0) {
    _raw_stream = sub_stream;
    _sub_stream = new CodedInputStream(_raw_stream);
    _sub_stream->SetTotalBytesLimit(pow(1024,3), pow(1024,3));
}

BlockCompressionInputStream::~BlockCompressionInputStream() {
    delete _sub_stream;
    delete[] _output_buffer;
}

bool BlockCompressionInputStream::Skip(int count) {
    assert(false);
};

void BlockCompressionInputStream::BackUp(int count) {
    _backed_up_bytes += count;
};

void BlockCompressionInputStream::reset_input_stream() {
    delete _sub_stream;
    _sub_stream = new CodedInputStream(_raw_stream);
    _sub_stream->SetTotalBytesLimit(pow(1024,3), pow(1024,3));
}

bool BlockCompressionInputStream::Next(const void** data, int* size) {
    if (_backed_up_bytes) {
        size_t skip = _output_buffer_size - _backed_up_bytes;
        assert(skip >= 0);
        (*data) = _output_buffer + skip;
        (*size) = _backed_up_bytes;
        _backed_up_bytes = 0;
        return true;
    }
    
    uint32_t compressed_size = 0;
    assert(_sub_stream->ReadVarint32(&compressed_size));
    assert(compressed_size < BLOCKSIZE*10);

    char * tempbuffer = new char[compressed_size];
    _sub_stream->ReadRaw(tempbuffer, compressed_size);
    RawUncompress(tempbuffer, compressed_size);
    delete[] tempbuffer;
    
    reset_input_stream(); // TODO: probably call this only every Limit/BLOCKSIZE
    
    (*size) = _output_buffer_size;
    (*data) = _output_buffer;
    return true;
}

// =========================================================================

BlockCompressionOutputStream::BlockCompressionOutputStream(
    ZeroCopyOutputStream* sub_stream)
    : _input_buffer(NULL), _sub_stream(new CodedOutputStream(sub_stream)), _backed_up_bytes(0),
      _byte_count(0) {}

BlockCompressionOutputStream::~BlockCompressionOutputStream() {
    if (_input_buffer) {
        // The buffer is not empty, there is stuff yet to be written.
        // This is necessary because we can't call virtual functions from any
        // destructor. Often this results in a pure virtual function call.
        // Call BlockCompressionOutputStream::Flush() before destroying 
        // this object.
        assert(false);
        // Flush();
    }
    delete _sub_stream;
}

void BlockCompressionOutputStream::BackUp(int count) {
    _backed_up_bytes += count;
}

bool BlockCompressionOutputStream::Flush()
{
    size_t size = BLOCKSIZE - _backed_up_bytes;
    if (!_input_buffer || size == 0) return true;
    
    size_t compressed_size = 0;
    char * compressed_data = new char[MaxCompressedLength(size)];
    compressed_size = RawCompress(_input_buffer, size, compressed_data);
    
    assert(compressed_size <= 2*BLOCKSIZE);
    
    uint32_t compressed_size_32 = static_cast<uint32_t>(compressed_size);
    _sub_stream->WriteVarint32(compressed_size_32);
    _sub_stream->WriteRaw(compressed_data, compressed_size_32);
    delete[] compressed_data;
        
    _backed_up_bytes = 0;
    delete[] _input_buffer;
    _input_buffer = 0;
    return true;
}

bool BlockCompressionOutputStream::Next(void** data, int* size) {
    if (_backed_up_bytes) {
        size_t skip = BLOCKSIZE - _backed_up_bytes;
        assert(skip >= 0);
        (*data) = _input_buffer + skip;
        (*size) = _backed_up_bytes;
        _backed_up_bytes = 0;
        return true;
    }
    if(_input_buffer) Flush();
    _input_buffer = new char[BLOCKSIZE];
    (*data) = _input_buffer;
    (*size) = BLOCKSIZE;
    return true;
}



#if HAVE_SNAPPY

/// Snappy implementation

void SnappyInputStream::RawUncompress(char* input_buffer, uint32_t compressed_size) {
    size_t uncompressed_size;
    bool success = ::snappy::GetUncompressedLength(
        input_buffer, compressed_size, &uncompressed_size);
    assert(success);
    
    if (uncompressed_size > _output_buffer_size) {
        delete[] _output_buffer;
        _output_buffer = new char[_output_buffer_size];
    }
    success = ::snappy::RawUncompress(input_buffer, compressed_size,
        _output_buffer);
    assert(success);    
}


uint32_t SnappyOutputStream::MaxCompressedLength(size_t input_size)
{
    return ::snappy::MaxCompressedLength(input_size);
}

uint32_t SnappyOutputStream::RawCompress(char* input_buffer, size_t input_size,
    char* output_buffer)
{
    size_t compressed_size = 0;
    ::snappy::RawCompress(input_buffer, input_size, output_buffer,
        &compressed_size);
    return compressed_size;
}

#endif  // HAVE_SNAPPY


/// LZ4 implementation

void LZ4InputStream::RawUncompress(char* input_buffer, uint32_t compressed_size) {
    if (not _output_buffer) {
        _output_buffer = new char[BLOCKSIZE];
    }
    _output_buffer_size = LZ4_uncompress_unknownOutputSize(input_buffer,
        _output_buffer, compressed_size, BLOCKSIZE);
}

uint32_t LZ4OutputStream::MaxCompressedLength(size_t input_size)
{
    return LZ4_compressBound(input_size);
}

uint32_t LZ4OutputStream::RawCompress(char* input_buffer, size_t input_size,
    char* output_buffer)
{
    assert(input_size);
    return ::LZ4_compress(input_buffer, output_buffer, input_size);
}

}; // namespace zerocc
