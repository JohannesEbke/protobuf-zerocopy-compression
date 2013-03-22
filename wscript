#! /usr/bin/env python

def options(opt):
    opt.load("compiler_c compiler_cxx waf_unit_test")

def configure(conf):
    conf.load("compiler_c compiler_cxx waf_unit_test")
    conf.check_cfg(package="protobuf", uselib_store="protobuf", args="--libs --cflags", mandatory=True)

def build(bld):
    bld.load("compiler_c compiler_cxx waf_unit_test")

    bld.stlib(source=["compressed_stream.cc", "block_wrappers.cc", "gzip_stream.cc", "lz4.cc"], 
            target="protobuf-zerocopy-compression", 
            use="protobuf",
            includes=".")

    bld.program(source="test.cc", 
            target="test",
            use="protobuf-zerocopy-compression protobuf", 
            includes=".")

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)

