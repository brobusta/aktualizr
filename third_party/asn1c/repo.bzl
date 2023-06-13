""" module to load asn1c """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_asn1c_repo():
    http_archive(
        name = "asn1c",
        build_file = "@//third_party/asn1c:asn1c.BUILD",
        sha256 = "d46922700805b8f144f40ba7828e991cafeff51beb48ebc3bbf8687337d23348",
        strip_prefix = "asn1c-9925dbbda86b436896108439ea3e0a31280a6065",
        urls = [
            "https://github.com/vlm/asn1c/archive/9925dbbda86b436896108439ea3e0a31280a6065.zip",
        ],
    )
