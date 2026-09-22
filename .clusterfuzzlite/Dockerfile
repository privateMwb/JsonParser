# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:809bff641a5be41a1482ff3a242501fe510df7bddcaa240ebc74c396cf796335

COPY . $SRC/JsonParser
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/JsonParser
