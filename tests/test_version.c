#include "unity.h"

#include <gldiagramdesktop/app.h>
#include <gldiagramdesktop/version.h>

#include <string.h>

void setUp(void) {}

void tearDown(void) {}

static void test_version_matches_generated_header(void) {
    TEST_ASSERT_NOT_NULL(gldiagramdesktop_version());
    TEST_ASSERT_EQUAL_STRING(GLDIAGRAMDESKTOP_VERSION_STRING, gldiagramdesktop_version());
}

static void test_version_is_semver(void) {
    const char *version = gldiagramdesktop_version();
    TEST_ASSERT_NOT_NULL(version);
    TEST_ASSERT_NOT_NULL(strchr(version, '.'));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_version_matches_generated_header);
    RUN_TEST(test_version_is_semver);
    return UNITY_END();
}
