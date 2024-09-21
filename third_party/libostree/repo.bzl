""" module to load libostree library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libostree_repo():
    http_archive(
        name = "libostree",
        build_file = "@//third_party/libostree:libostree.BUILD",
        sha256 = "bb241e6de33f90bd7f86d41df7eb445a7e7d75f89f3554a1394f3bdb2266c86d",
        strip_prefix = "ostree-2023.8",
        urls = [
            "https://github.com/ostreedev/ostree/archive/refs/tags/v2023.8.tar.gz",
        ],
    )
