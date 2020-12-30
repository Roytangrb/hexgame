load("@rules_cc//cc:defs.bzl", "cc_library")

package(
  default_visibility = [
    "//main:__pkg__",
    "//test:__pkg__",
  ]
)

cc_library(
  name = "hexboard",
  srcs = ["hexboard.cc"],
  hdrs = ["hexboard.h"],
)

cc_library(
  name = "hexgame",
  srcs = ["hexgame.cc"],
  hdrs = ["hexgame.h"],
  deps = [":hexboard"],
)

cc_library(
  name = "hexai",
  srcs = ["hexai.cc"],
  hdrs = ["hexai.h"],
  deps = [
    ":hexboard",
    ":hexgame",
  ],
)
