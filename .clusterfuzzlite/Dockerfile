# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:e8214e916e787f6109ef49983d97a72ea9062e1b26b7fe1d4661d1d2ff5fe276

COPY . $SRC/JsonParser
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/JsonParser
