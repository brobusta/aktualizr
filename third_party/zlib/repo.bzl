""" module to load zlib library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_zlib_repo():
    http_archive(
        name = "zlib",
        build_file = "@//third_party/zlib:zlib.BUILD",
        sha256 = "b5b06d60ce49c8ba700e0ba517fa07de80b5d4628a037f4be8ad16955be7a7c0",
        strip_prefix = "zlib-1.3",
        urls = [
            "https://github.com/madler/zlib/archive/refs/tags/v1.3.1.tar.gz",
        ],
    )
