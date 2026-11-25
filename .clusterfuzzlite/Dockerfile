# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:46319ce66bf966bda1f47fdfcfb64b896e0311d3d2243bffb3a8f8b0916c1338

COPY . $SRC/JsonParser
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/JsonParser
