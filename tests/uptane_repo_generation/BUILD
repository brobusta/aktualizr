load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

package(default_visibility = ["//visibility:public"])

sh_binary(
    name = "generate_repo",
    srcs = ["generate_repo.sh"],
)

genrule(
    name = "uptane_repo_full_no_correlation_id",
    outs = [
        "uptane_repos/full_no_correlation_id/keys/director/timestamp/public.key",
        "uptane_repos/full_no_correlation_id/keys/director/timestamp/private.key",
        "uptane_repos/full_no_correlation_id/keys/director/timestamp/key_type",
        "uptane_repos/full_no_correlation_id/keys/director/root/public.key",
        "uptane_repos/full_no_correlation_id/keys/director/root/private.key",
        "uptane_repos/full_no_correlation_id/keys/director/root/key_type",
        "uptane_repos/full_no_correlation_id/keys/director/snapshot/public.key",
        "uptane_repos/full_no_correlation_id/keys/director/snapshot/private.key",
        "uptane_repos/full_no_correlation_id/keys/director/snapshot/key_type",
        "uptane_repos/full_no_correlation_id/keys/director/targets/public.key",
        "uptane_repos/full_no_correlation_id/keys/director/targets/private.key",
        "uptane_repos/full_no_correlation_id/keys/director/targets/key_type",
        "uptane_repos/full_no_correlation_id/keys/image/timestamp/public.key",
        "uptane_repos/full_no_correlation_id/keys/image/timestamp/private.key",
        "uptane_repos/full_no_correlation_id/keys/image/timestamp/key_type",
        "uptane_repos/full_no_correlation_id/keys/image/root/public.key",
        "uptane_repos/full_no_correlation_id/keys/image/root/private.key",
        "uptane_repos/full_no_correlation_id/keys/image/root/key_type",
        "uptane_repos/full_no_correlation_id/keys/image/snapshot/public.key",
        "uptane_repos/full_no_correlation_id/keys/image/snapshot/private.key",
        "uptane_repos/full_no_correlation_id/keys/image/snapshot/key_type",
        "uptane_repos/full_no_correlation_id/keys/image/targets/public.key",
        "uptane_repos/full_no_correlation_id/keys/image/targets/private.key",
        "uptane_repos/full_no_correlation_id/keys/image/targets/key_type",
        "uptane_repos/full_no_correlation_id/repo/director/manifest",
        "uptane_repos/full_no_correlation_id/repo/director/root.json",
        "uptane_repos/full_no_correlation_id/repo/director/snapshot.json",
        "uptane_repos/full_no_correlation_id/repo/director/timestamp.json",
        "uptane_repos/full_no_correlation_id/repo/director/1.root.json",
        "uptane_repos/full_no_correlation_id/repo/director/targets.json",
        "uptane_repos/full_no_correlation_id/repo/repo/root.json",
        "uptane_repos/full_no_correlation_id/repo/repo/snapshot.json",
        "uptane_repos/full_no_correlation_id/repo/repo/timestamp.json",
        "uptane_repos/full_no_correlation_id/repo/repo/1.root.json",
        "uptane_repos/full_no_correlation_id/repo/repo/targets.json",
        "uptane_repos/full_no_correlation_id/repo/repo/targets/primary.txt",
        "uptane_repos/full_no_correlation_id/repo/repo/targets/secondary.txt",
    ],
    cmd = (
        "$(location :generate_repo) " +
        "$(location //src/uptane_generator) " +
        "$(RULEDIR)/uptane_repos/full_no_correlation_id " +
        "--add_default_secondary"
    ),
    tools = [
        ":generate_repo",
        "//src/uptane_generator",
    ],
)
