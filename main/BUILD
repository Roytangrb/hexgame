load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
  name = "hexboard",
  srcs = ["Board.cc"],
  hdrs = ["Board.h"],
)

cc_binary(
  name = "hexgame",
  srcs = ["hexgame.cc"],
  deps = [
    ":hexboard",
  ],
)
