/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
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

#include "mgos_i2c.h"

static struct mgos_i2c *s_global_i2c;

int mgos_i2c_read_reg_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg) {
  uint8_t value;
  if (!mgos_i2c_read_reg_n(conn, addr, reg, 1, &value)) {
    return -1;
  }
  return value;
}

int mgos_i2c_read_reg_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg) {
  uint8_t tmp[2];
  if (!mgos_i2c_read_reg_n(conn, addr, reg, 2, tmp)) {
    return -1;
  }
  return (((uint16_t) tmp[0]) << 8) | tmp[1];
}

bool mgos_i2c_read_reg_n(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
                         size_t n, uint8_t *buf) {
  return mgos_i2c_write(conn, addr, &reg, 1, false /* stop */) &&
         mgos_i2c_read(conn, addr, buf, n, true /* stop */);
}

bool mgos_i2c_write_reg_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
                          uint8_t value) {
  uint8_t tmp[2] = {reg, value};
  return mgos_i2c_write(conn, addr, tmp, sizeof(tmp), true /* stop */);
}

bool mgos_i2c_write_reg_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
                          uint16_t value) {
  uint8_t tmp[3] = {reg, (uint8_t)(value >> 8), (uint8_t) value};
  return mgos_i2c_write(conn, addr, tmp, sizeof(tmp), true /* stop */);
}

bool mgos_i2c_write_reg_n(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
                          size_t n, const uint8_t *buf) {
  bool res = false;
  uint8_t *tmp = calloc(n + 1, 1);
  if (tmp) {
    *tmp = reg;
    memcpy(tmp + 1, buf, n);
    res = mgos_i2c_write(conn, addr, tmp, n + 1, true /* stop */);
    free(tmp);
  }
  return res;
}

int mgos_i2c_read_reg_bit_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
														uint8_t bitNum) {
   	int result = -1;
    int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
   		uint8_t b = (uint8_t) val;
			result = b & (1 << bitNum);
    }
    return result;
}

int mgos_i2c_read_reg_bit_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
														uint8_t bitNum) {
   	int result = -1;
    int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
   		uint16_t w = (uint16_t) val;
			result = w & (1 << bitNum);
    }
    return result;
}

int mgos_i2c_read_reg_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
														 uint8_t bitStart, uint8_t length) {
   	int result = -1;
    int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
    		uint8_t b = (uint8_t) val;
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        result = b;
    }
    return result;
}

int mgos_i2c_read_reg_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
														 uint8_t bitStart, uint8_t length) {
   	int result = -1;
    int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = (uint16_t) val;
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        result = w;
    }
    return result;
}

int mgos_i2c_read_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t bitStart,
												 uint8_t length, bool stop) {
   	int result = -1;
   	uint8_t val;
    if (mgos_i2c_read(conn, addr, &val, 1, stop)) {
    		uint8_t b = (uint8_t) val;
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        result = b;
    }
    return result;
}

int mgos_i2c_read_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t bitStart,
												 uint8_t length, bool stop) {
   	int result = -1;
   	uint16_t val;
    if (mgos_i2c_read(conn, addr, &val, 2, stop)) {
    		uint16_t w = (uint16_t) val;
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        result = w;
    }
    return result;
}

bool mgos_i2c_write_reg_bit_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
															uint8_t bitNum, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
    		uint8_t b = (uint8_t) val;
		    b = (value != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
		    result = mgos_i2c_write_reg_b(conn, addr, reg, b);
		}
		return result;
}

bool mgos_i2c_write_reg_bit_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
															uint8_t bitNum, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = (uint16_t) val;
		    w = (value != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
		    result = mgos_i2c_write_reg_w(conn, addr, reg, w);
		}
		return result;
}

bool mgos_i2c_write_reg_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg,
															 uint8_t bitStart, uint8_t length, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
    		uint8_t b = mgos_calc_bits_b((uint8_t) val, bitStart, length, value); 	
		    result = mgos_i2c_write_reg_b(conn, addr, reg, b);
		}
		return result;
}

bool mgos_i2c_write_reg_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, 
															 uint8_t bitStart, uint8_t length, uint16_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = mgos_calc_bits_w((uint16_t) val, bitStart, length, value); 	
		    result = mgos_i2c_write_reg_w(conn, addr, reg, w);
		}
		return result;
}

bool mgos_i2c_write_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t bitStart,
													 uint8_t length, uint8_t value, bool stop) {
/*
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - addr: 0x%x", addr));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - bitStart: %d", bitStart));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - length: %d", length));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - value: 0x%x", value));
*/
	bool result = false;
	static uint8_t b;
	if (mgos_i2c_read(conn, addr, &b, 1, stop)) {
// LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - b: 0x%x", b));
			b = mgos_calc_bits_b(b, bitStart, length, value); 	
 //LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - b: 0x%x", b));
			result = mgos_i2c_write(conn, addr, &b, 1, stop);
	}
// LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_b - result: %s", result ? "true" : "false"));
	return result;
}

bool mgos_i2c_write_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t bitStart,
													 uint8_t length, uint16_t value, bool stop) {
/*
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - addr: 0x%x", addr));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - bitStart: %d", bitStart));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - length: %d", length));
 LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - value: 0x%x", value));
*/
		bool result = false;
   	static uint16_t w;
    if (mgos_i2c_read(conn, addr, &w, 2, stop)) {
// LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - w: 0x%x", w));
				w = mgos_calc_bits_w(w, bitStart, length, value); 	
// LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - w: 0x%x", w));
        result = mgos_i2c_write(conn, addr, &w, 2, stop);
		}
// LOG(LL_ERROR, ("I2C mgos_i2c_write_bits_w - result: %s", result ? "true" : "false"));
		return result;
}

uint8_t mgos_calc_bits_b(uint8_t b, uint8_t bitStart, uint8_t length, uint8_t value) {
/*
 LOG(LL_ERROR, ("I2C mgos_calc_bits_b - b: 0x%x", b));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_b - bitStart: %d", bitStart));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_b - length: %d", length));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_b - value: 0x%x", value));
*/
	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
// LOG(LL_ERROR, ("I2C mgos_calc_bits_b - mask: 0x%x", mask));
  value <<= (bitStart - length + 1); 
// LOG(LL_ERROR, ("I2C mgos_calc_bits_b - value: 0x%x", value));
  value &= mask;
// LOG(LL_ERROR, ("I2C mgos_calc_bits_b - value: 0x%x", value));
  b &= ~(mask);
// LOG(LL_ERROR, ("I2C mgos_calc_bits_b - b: 0x%x", b));
  b |= value;
// LOG(LL_ERROR, ("I2C mgos_calc_bits_b - b: 0x%x", b));
	return b;
}

uint16_t mgos_calc_bits_w(uint16_t w, uint8_t bitStart, uint8_t length, uint16_t value) {
/*
 LOG(LL_ERROR, ("I2C mgos_calc_bits_w - w: 0x%x", w));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_w - bitStart: %d", bitStart));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_w - length: %d", length));
 LOG(LL_ERROR, ("I2C mgos_calc_bits_w - value: 0x%x", value));
*/
	uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
// LOG(LL_ERROR, ("I2C mgos_calc_bits_w - mask: 0x%x", mask));
  value <<= (bitStart - length + 1); 
// LOG(LL_ERROR, ("I2C mgos_calc_bits_w - value: 0x%x", value));
  value &= mask;
// LOG(LL_ERROR, ("I2C mgos_calc_bits_w - value: 0x%x", value));
  w &= ~(mask);
// LOG(LL_ERROR, ("I2C mgos_calc_bits_w - w: 0x%x", w));
  w |= value;
// LOG(LL_ERROR, ("I2C mgos_calc_bits_w - w: 0x%x", w));
	return w;
}

bool mgos_i2c_init(void) {
  if (!mgos_sys_config_get_i2c_enable()) return true;
  s_global_i2c = mgos_i2c_create(mgos_sys_config_get_i2c());
  return (s_global_i2c != NULL);
}

struct mgos_i2c *mgos_i2c_get_global(void) {
  return s_global_i2c;
}
