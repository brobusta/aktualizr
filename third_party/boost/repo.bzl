"""
Load boost rules
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_boost_repo():
    http_archive(
        name = "com_github_nelhage_rules_boost",
        sha256 = "c4699f38eb81e17cf5130bcd2520ddff3d76648025f261c2eb3e3bef4e41d20b",
        strip_prefix = "rules_boost-40ce3bdc3d2f947aa3421b38ae4d3e8bd3ace7fc",
        urls = [
            "https://github.com/nelhage/rules_boost/archive/40ce3bdc3d2f947aa3421b38ae4d3e8bd3ace7fc.tar.gz",
        ],
    )
