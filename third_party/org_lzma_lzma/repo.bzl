""" module to load xz library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_lzma_repo():
    http_archive(
        name = "org_lzma_lzma",
        build_file = "@//third_party/org_lzma_lzma:lzma.BUILD",
        url = "https://github.com/tukaani-project/xz/releases/download/v5.4.5/xz-5.4.5.tar.gz",
        sha256 = "135c90b934aee8fbc0d467de87a05cb70d627da36abe518c357a873709e5b7d6",
        strip_prefix = "xz-5.4.5",
    )
