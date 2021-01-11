load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def json_spirit_deps():
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        git_repository(
            name = "com_github_nelhage_rules_boost",
            commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
            remote = "https://github.com/nelhage/rules_boost",
            shallow_since = "1591047380 -0700",
        )
