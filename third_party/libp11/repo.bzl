""" module to load libp11 library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libp11_repo():
    http_archive(
        name = "libp11",
        build_file = "@//third_party/libp11:libp11.BUILD",
        sha256 = "c1a233ecb09ab48595ba36a504c47f82cf53a59aa8db789c02ca72d50e85ca87",
        strip_prefix = "libp11-libp11-0.4.12",
        urls = [
            "https://github.com/OpenSC/libp11/archive/refs/tags/libp11-0.4.12.tar.gz",
        ],
    )
