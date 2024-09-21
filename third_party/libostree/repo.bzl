""" module to load libostree library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libostree_repo():
    http_archive(
        name = "libostree",
        build_file = "@//third_party/libostree:libostree.BUILD",
        sha256 = "369b5aad48b0322206ac51687ffaebf7f38d6384a35ef3ee2ccb9e7c0447d03a",
        strip_prefix = "ostree-2024.8",
        urls = [
            "https://github.com/ostreedev/ostree/archive/refs/tags/v2024.8.tar.gz",
        ],
    )
