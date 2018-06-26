/*
 * Copyright (c) 2018 Manfred Mueller-Spaeth <fms1961@gmail.com>
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos_bitops.h"

uint8_t mgos_set_bit_b(bool doSet, uint8_t bitPos, uint8_t b) {
	if (doSet) {
		return (b | (1 << bitPos));
	} else {
		return (b & ~(1 << bitPos));
	}
}

uint16_t mgos_set_bit_w(bool doSet, uint8_t bitPos, uint16_t w) {
	if (doSet) {
		return (w | (1 << bitPos));
	} else {
		return (w & ~(1 << bitPos));
	}
}

bool mgos_get_bit_b(uint8_t b, uint8_t bitPos) {
	return (b & (1 << bitPos));
}

bool mgos_get_bit_w(uint16_t w, uint8_t bitPos) {
	return (w & (1 << bitPos));
}

uint8_t mgos_get_bits_b(uint8_t b, uint8_t bitStart, uint8_t length, uint8_t value) {
	uint8_t b = value;
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  b &= mask;
  b >>= (bitStart - length + 1);
  return b;
}

uint16_t mgos_get_bits_w(uint16_t w, uint8_t bitStart, uint8_t length, uint16_t value) {
	uint16_t w = value;
  uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  w &= mask;
  w >>= (bitStart - length + 1);
  return w;
}

uint8_t mgos_set_bits_b(uint8_t b, uint8_t bitStart, uint8_t length, uint8_t value) {
	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  value <<= (bitStart - length + 1); 
  value &= mask;
  b &= ~(mask);
  b |= value;
	return b;
}

uint16_t mgos_set_bits_w(uint16_t w, uint8_t bitStart, uint8_t length, uint16_t value) {
	uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  value <<= (bitStart - length + 1); 
  value &= mask;
  w &= ~(mask);
  w |= value;
	return w;
}

bool mgos_bitops_init(void) {
  return true;
}