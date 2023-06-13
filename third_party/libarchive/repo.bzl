""" module to load libarchive library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libarchive_repo():
    http_archive(
        name = "libarchive",
        build_file = "@//third_party/libarchive:libarchive.BUILD",
        sha256 = "652b84588488c2ff38db8f666cd7f781163f85bff4449dcb2e16d3c734f96697",
        strip_prefix = "libarchive-3.6.2",
        urls = [
            "https://github.com/libarchive/libarchive/archive/refs/tags/v3.6.2.tar.gz",
        ],
    )
