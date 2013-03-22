#! /usr/bin/env python

def go(ctx):
    from waflib.Options import commands, options
    commands += ["configure", "build"]

def options(opt):
    opt.load("compiler_c compiler_cxx waf_unit_test")

def configure(conf):
    conf.load("compiler_c compiler_cxx waf_unit_test")
    conf.check_cfg(package="protobuf", uselib_store="protobuf", args="--libs --cflags", mandatory=True)
    conf.check_cxx(lib="snappy", uselib_store="snappy", mandatory=False, define_name="HAVE_SNAPPY")
    conf.write_config_header('config.h')

def build(bld):
    bld.load("compiler_c compiler_cxx waf_unit_test")

    bld.stlib(source=["compressed_stream.cc", "block_wrappers.cc", "gzip_stream.cc", "lz4.cc"], 
            target="protobuf-zerocopy-compression", 
            use="protobuf snappy",
            includes=".")

    bld.program(source="test.cc", 
            target="test",
            use="protobuf-zerocopy-compression protobuf snappy",
            includes=".")

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)

