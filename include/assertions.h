#pragma once

#include <assert.h>

#define ASSERT_SIZE(structure, size) static_assert(sizeof(structure) == size, #structure " must be " #size " bytes")
