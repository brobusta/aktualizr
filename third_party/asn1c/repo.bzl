""" module to load asn1c """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_asn1c_repo():
    http_archive(
        name = "asn1c",
        build_file = "@//third_party/asn1c:asn1c.BUILD",
        sha256 = "b0f58fb5579ba93249d0a5a071c028404c5040f468a27dfe339a7d677d3545b9",
        strip_prefix = "asn1c-8a274c3f99761f249b6c205e1c89be1b1cb7c669",
        urls = [
            "https://github.com/vlm/asn1c/archive/8a274c3f99761f249b6c205e1c89be1b1cb7c669.zip",
        ],
    )
