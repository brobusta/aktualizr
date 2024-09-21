""" module to load xz library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_lzma_repo():
    http_archive(
        name = "org_lzma_lzma",
        build_file = "@//third_party/org_lzma_lzma:lzma.BUILD",
        url = "https://github.com/tukaani-project/xz/releases/download/v5.6.2/xz-5.6.2.tar.gz",
        sha256 = "8bfd20c0e1d86f0402f2497cfa71c6ab62d4cd35fd704276e3140bfb71414519",
        strip_prefix = "xz-5.6.2",
    )
