#pragma once

#include <stdint.h>
#include <stdlib.h>

#define KEY_LENGTH 6u

int GenerateKey(uint8_t* key, const uint8_t* seed, uint8_t wanted_security_level);
