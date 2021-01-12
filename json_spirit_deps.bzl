load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def json_spirit_deps():
    """ json_spirit_deps fetchs all dependencies for this repository """
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        git_repository(
            name = "com_github_nelhage_rules_boost",
            commit = "5cff96018ec4662cc61268cf248edfc6e6fe4635",
            remote = "https://github.com/nelhage/rules_boost",
            shallow_since = "1591047380 -0700",
        )
