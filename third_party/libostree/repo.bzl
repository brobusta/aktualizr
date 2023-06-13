""" module to load libostree library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libostree_repo():
    http_archive(
        name = "libostree",
        build_file = "@//third_party/libostree:libostree.BUILD",
        sha256 = "2f1a5a89efe3c0794eafb2a1b3048eb4ce8e6f46dd24791cd2455ab7c0f61da2",
        strip_prefix = "ostree-2023.3",
        urls = [
            "https://github.com/ostreedev/ostree/archive/refs/tags/v2023.3.tar.gz",
        ],
    )
