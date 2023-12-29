"""
Load boost rules
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_boost_repo():
    http_archive(
        name = "com_github_nelhage_rules_boost",
        sha256 = "1ef96e5c4c7c05024e60dc5927f7c1f39c692530d1a396f0968ce1715cd00df4",
        strip_prefix = "rules_boost-e72eb259976357f6e82f4d74d74a7c12d1c3776d",
        urls = [
            "https://github.com/nelhage/rules_boost/archive/e72eb259976357f6e82f4d74d74a7c12d1c3776d.tar.gz",
        ],
    )
