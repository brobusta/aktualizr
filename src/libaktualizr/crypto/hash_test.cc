#include <gtest/gtest.h>

#include "crypto.h"
#include "logging/logging.h"

TEST(Hash, EncodeDecode) {
  std::vector<Hash> hashes = {{Hash::Type::kSha256, "abcd"}, {Hash::Type::kSha512, "defg"}};

  std::string encoded = Hash::encodeVector(hashes);
  std::vector<Hash> decoded = Hash::decodeVector(encoded);

  EXPECT_EQ(hashes, decoded);
}

TEST(Hash, DecodeBad) {
  std::string bad1 = ":";
  EXPECT_EQ(Hash::decodeVector(bad1), std::vector<Hash>{});

  std::string bad2 = ":abcd;sha256:12";
  EXPECT_EQ(Hash::decodeVector(bad2), std::vector<Hash>{Hash(Hash::Type::kSha256, "12")});

  std::string bad3 = "sha256;";
  EXPECT_EQ(Hash::decodeVector(bad3), std::vector<Hash>{});

  std::string bad4 = "sha256:;";
  EXPECT_EQ(Hash::decodeVector(bad4), std::vector<Hash>{});
}

#ifndef __NO_MAIN__
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  logger_set_threshold(spdlog::level::trace);
  return RUN_ALL_TESTS();
}
#endif
