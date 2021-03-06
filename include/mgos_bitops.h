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

/*
 * Functions providing bit operations.
 *
 */

#ifndef MMS_LIBS_SRC_MGOS_BITOPS_H_
#define MMS_LIBS_SRC_MGOS_BITOPS_H_

#include "mgos.h"
#include "mgos_features.h"

#include <stdbool.h>
#include <stdint.h>

#include "mgos_init.h"
#include "mgos_sys_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t mgos_set_bit_b(bool doSet, uint8_t bitPos, uint8_t b);

uint16_t mgos_set_bit_w(bool doSet, uint8_t bitPos, uint16_t w);

bool mgos_get_bit_b(uint8_t b, uint8_t bitPos);

bool mgos_get_bit_w(uint16_t w, uint8_t bitPos);

uint8_t mgos_get_bits_b(uint8_t b, uint8_t bitStart, uint8_t length, uint8_t value);

uint16_t mgos_get_bits_w(uint16_t w, uint8_t bitStart, uint8_t length, uint16_t value);

uint8_t mgos_set_bits_b(uint8_t b, uint8_t bitStart, uint8_t length, uint8_t value);

uint16_t mgos_set_bits_w(uint16_t w, uint8_t bitStart, uint8_t length, uint16_t value);

bool mgos_bitops_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MMS_LIBS_SRC_MGOS_BITOPS_H_ */
