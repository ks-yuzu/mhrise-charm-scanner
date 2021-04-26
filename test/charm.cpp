#include <gtest/gtest.h>
#include "../lib/charm.hpp"


TEST(dump, 1) {
  testing::internal::CaptureStdout();

  Charm charm(
    "神嵐の護石",
    7,
    { CharmSlot(3), CharmSlot(2), CharmSlot(2) },
    { Skill("見切り", 2) }
  );
  std::cout << charm << std::endl;

  std::string stdout = testing::internal::GetCapturedStdout();

  EXPECT_STREQ("神嵐の護石 7 3-2-2 見切り2\n", stdout.c_str());
}


TEST(dump, 2) {
  testing::internal::CaptureStdout();

  Charm charm(
    "神嵐の護石",
    7,
    { CharmSlot(1) },
    { Skill("攻撃", 3) }
  );
  std::cout << charm << std::endl;

  std::string stdout = testing::internal::GetCapturedStdout();

  EXPECT_STREQ("神嵐の護石 7 1-0-0 攻撃3\n", stdout.c_str());
}


int main(void) {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
