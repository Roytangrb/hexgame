load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
  name = "hexboard",
  srcs = ["Board.cc"],
  hdrs = ["Board.h"],
)

cc_library(
  name = "hexgame",
  srcs = ["Game.cc"],
  hdrs = ["Game.h"],
  deps = [":hexboard"],
)

cc_library(
  name = "hexai",
  srcs = ["AI.cc"],
  hdrs = ["AI.h"],
  deps = [
    ":hexboard",
    ":hexgame",
  ],
)

cc_binary(
  name = "hex",
  srcs = ["hex.cc"],
  deps = [
    ":hexboard",
    ":hexgame",
    ":hexai",
  ],
  visibility = ["//test:__pkg__",],
)
