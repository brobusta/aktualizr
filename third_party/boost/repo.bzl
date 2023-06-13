"""
Load boost rules
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_boost_repo():
    http_archive(
        name = "com_github_nelhage_rules_boost",
        sha256 = "a7883b3d30653dd4bc6dc3ea1046e3f4515492d709a83bee8ead50f3ac9d8389",
        strip_prefix = "rules_boost-623f5cb627e06851cd8378051fb3eb16bce487d4",
        urls = [
            "https://github.com/nelhage/rules_boost/archive/623f5cb627e06851cd8378051fb3eb16bce487d4.tar.gz",
        ],
    )
