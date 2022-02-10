workspace(name = "com_github_mingkaic_cisab")

# === development ===

load("//third_party:all.bzl", "test_dependencies")
test_dependencies()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
    name = "com_grail_bazel_compdb",
	remote = "https://github.com/grailbio/bazel-compilation-database",
	tag = "0.4.5",
)
