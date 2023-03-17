load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def gtest_repository(name):
    print("downloading gtest v1.13.0 from cisab")
    git_repository(
        name = name,
        remote = "https://github.com/google/googletest",
		tag = "v1.13.0",
    )
