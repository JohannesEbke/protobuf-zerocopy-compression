#include <string>
#include <iostream>

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <compressed_stream.h>

int main() {
    std::string text = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet!";
    std::string compressed, out_text;

    google::protobuf::io::StringOutputStream o(&compressed);
    zerocc::AbstractCompressedOutputStream * co = get_compressed_output_stream(&o, zerocc::ZLIB, 1);
    {
        google::protobuf::io::CodedOutputStream c(co);
        c.WriteVarint32(text.size());
        c.WriteString(text);
    }
    co->Flush();
    delete co;

    std::cout << text << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << compressed << std::endl;
    std::cout << "-----------------------------" << std::endl;

    google::protobuf::io::ArrayInputStream i(&compressed[0], compressed.size());
    zerocc::AbstractCompressedInputStream * ci = get_compressed_input_stream(&i, zerocc::ZLIB);
    {
        google::protobuf::io::CodedInputStream c(ci);
        uint32_t size;
        c.ReadVarint32(&size);
        c.ReadString(&out_text, size);
    }
    std::cout << out_text << std::endl;
    std::cout << "ExpectAtEnd is " << ci->ExpectAtEnd() << " (should be 1)" << std::endl;
    delete ci;
    return 0;
}
