#include <compressed_stream.h>
#include <gzip_stream.h>
#include <block_wrappers.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace zerocc {

AbstractCompressedInputStream * get_compressed_input_stream(ZeroCopyInputStream * istream, CompressionType t) {
    switch(t) {
        case ZLIB:
            return new GzipInputStream(istream, GzipInputStream::ZLIB);
        case GZIP:
            return new GzipInputStream(istream, GzipInputStream::GZIP);
        case SNAPPY:
#ifdef HAVE_SNAPPY
            return new SnappyInputStream(istream);
#else 
            return NULL;
#endif
        case LZ4:
            return new LZ4InputStream(istream);
    }
    return NULL;
}

AbstractCompressedOutputStream * get_compressed_output_stream(ZeroCopyOutputStream * ostream, CompressionType t, int level) {
    switch (t) {
        case ZLIB:
            {
                GzipOutputStream::Options o;
                o.format = GzipOutputStream::ZLIB;
                o.compression_level = level;
                return new GzipOutputStream(ostream, o);
            }
        case GZIP:
            {
                GzipOutputStream::Options o;
                o.format = GzipOutputStream::GZIP;
                o.compression_level = level;
                return new GzipOutputStream(ostream, o);
            }
        case SNAPPY:
#ifdef HAVE_SNAPPY
            return new SnappyOutputStream(ostream);
#else
            return NULL;
#endif
        case LZ4:
            return new LZ4OutputStream(ostream);
    }
    return NULL;
}

}; // namespace zerocc
