""" module to load xz library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_lzma_repo():
    http_archive(
        name = "org_lzma_lzma",
        build_file = "@//third_party/org_lzma_lzma:lzma.BUILD",
        url = "https://github.com/tukaani-project/xz/releases/download/v5.4.3/xz-5.4.3.tar.gz",
        sha256 = "1c382e0bc2e4e0af58398a903dd62fff7e510171d2de47a1ebe06d1528e9b7e9",
        strip_prefix = "xz-5.4.3",
    )
