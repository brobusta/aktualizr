load("//third_party/asn1c:repo.bzl", "load_asn1c_repo")
load("//third_party/libp11:repo.bzl", "load_libp11_repo")
load("//third_party/libsodium:repo.bzl", "load_libsodium_repo")
load("//third_party/openssl:repo.bzl", "load_openssl_repo")

def _load_external_repos_impl(_ctx):
    """ module to load all external libraries """
    load_libsodium_repo()
    load_libp11_repo()
    load_openssl_repo()
    load_asn1c_repo()

load_external_repos = module_extension(
    implementation = _load_external_repos_impl,
)
