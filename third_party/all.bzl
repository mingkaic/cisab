load("//third_party:repos/gtest.bzl", "gtest_repository")

def validate(dep, excludes, force_includes):
    return dep not in excludes or dep in force_includes

def test_dependencies(excludes = [], force_include = []):
    ignores = native.existing_rules().keys() + excludes

    if validate("gtest", ignores, force_includes):
        gtest_repository(name = "gtest")
