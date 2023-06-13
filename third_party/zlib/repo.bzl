""" module to load zlib library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_zlib_repo():
    http_archive(
        name = "zlib",
        build_file = "@//third_party/zlib:zlib.BUILD",
        sha256 = "1525952a0a567581792613a9723333d7f8cc20b87a81f920fb8bc7e3f2251428",
        strip_prefix = "zlib-1.2.13",
        urls = [
            "https://github.com/madler/zlib/archive/refs/tags/v1.2.13.tar.gz",
        ],
    )
