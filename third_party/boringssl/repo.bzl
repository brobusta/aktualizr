""" module to load boringssl library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_boringssl_repo():
    http_archive(
        name = "boringssl",
        patch_args = ["-p1"],
        patches = ["@//third_party/boringssl:patch"],
        # sha256 = "68fb11cff451d91ba6adf684760d8348b72f2990c577a446b3b659babe28ca0e",
        strip_prefix = "boringssl-902ac31a2494b4aaeef4fab0f24965cc596198de",
        urls = [
            "https://github.com/google/boringssl/archive/902ac31a2494b4aaeef4fab0f24965cc596198de.tar.gz",
        ],
    )
