load("@rules_cc//cc:defs.bzl", "cc_binary")

package(
  default_visibility = ["//test:__pkg__"],
)

cc_binary(
  name = "hex",
  srcs = ["hex.cc"],
  deps = [
    "//lib:hexboard",
    "//lib:hexgame",
    "//lib:hexai",
  ],
)
