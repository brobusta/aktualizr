""" module to load sodium library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libsodium_repo():
    http_archive(
        name = "libsodium",
        sha256 = "d59323c6b712a1519a5daf710b68f5e7fde57040845ffec53850911f10a5d4f4",
        strip_prefix = "libsodium-1.0.18",
        build_file = "@//third_party/libsodium:libsodium.BUILD",
        urls = [
            "https://github.com/jedisct1/libsodium/archive/refs/tags/1.0.18.tar.gz",
        ],
    )
