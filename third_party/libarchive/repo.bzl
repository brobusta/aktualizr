""" module to load libarchive library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libarchive_repo():
    http_archive(
        name = "libarchive",
        build_file = "@//third_party/libarchive:libarchive.BUILD",
        sha256 = "2a27a429dde8a6920973a609f13e0d1f94b45a2c40c9f49fc66f7fd915e15b79",
        strip_prefix = "libarchive-3.7.6",
        urls = [
            "https://github.com/libarchive/libarchive/archive/refs/tags/v3.7.6.tar.gz",
        ],
    )
