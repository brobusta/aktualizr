load("//third_party/libsodium:repo.bzl", "load_libsodium_repo")
load("//third_party/libp11:repo.bzl", "load_libp11_repo")
load("//third_party/openssl:repo.bzl", "load_openssl_repo")
load("//third_party/libostree:repo.bzl", "load_libostree_repo")
load("//third_party/asn1c:repo.bzl", "load_asn1c_repo")

def load_external_repos():
    """ module to load all external libraries """
    load_libsodium_repo()
    load_libp11_repo()
    load_openssl_repo()
    load_libostree_repo()
    load_asn1c_repo()
