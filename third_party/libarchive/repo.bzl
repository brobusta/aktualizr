""" module to load libarchive library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libarchive_repo():
    http_archive(
        name = "libarchive",
        build_file = "@//third_party/libarchive:libarchive.BUILD",
        sha256 = "79b012035506c709495e982c36248253337c88e2e162636ec0505edff40f00d1",
        strip_prefix = "libarchive-3.7.5",
        urls = [
            "https://github.com/libarchive/libarchive/archive/refs/tags/v3.7.5.tar.gz",
        ],
    )
