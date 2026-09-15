#!/bin/bash -eu
# ============================================================
# .clusterfuzzlite/build.sh
#
# Unlike a header-only library, JsonParser's implementation lives in
# src/JsonPro/*.cpp (mirroring include/JsonPro/*.h), so those
# translation units need to be compiled once and linked into every
# harness -- they aren't pulled in just by including the headers.
#
# Add more `${SRC}/JsonParser/fuzz/fuzz_*.cpp` harnesses here as
# they're added; each becomes its own $OUT binary, all linked
# against the same set of library object files.
# ============================================================

cd "${SRC}/JsonParser"

LIB_OBJS=()
for src in src/JsonPro/Json.cpp src/JsonPro/Parser.cpp src/JsonPro/JsonObject.cpp; do
  obj="${WORK}/$(basename "${src}" .cpp).o"
  $CXX $CXXFLAGS -std=c++20 \
    -I"${SRC}/JsonParser/include" \
    -c "${src}" -o "${obj}"
  LIB_OBJS+=("${obj}")
done

$CXX $CXXFLAGS -std=c++20 \
  -I"${SRC}/JsonParser/include" \
  fuzz/fuzz_json.cpp \
  "${LIB_OBJS[@]}" \
  $LIB_FUZZING_ENGINE \
  -o "${OUT}/fuzz_json"
