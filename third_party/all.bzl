load("//third_party:repos/gtest.bzl", "gtest_repository")

def test_dependencies(excludes = []):
    ignores = native.existing_rules().keys() + excludes

    if "gtest" not in ignores:
        gtest_repository(name = "gtest")
