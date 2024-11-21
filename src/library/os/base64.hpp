/*
 * base64.hpp
 *
 *  Created on: Nov 21, 2024
 *      Author: DB
 */

#ifndef SRC_LIBRARY_OS_BASE64_HPP_
#define SRC_LIBRARY_OS_BASE64_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include "../base/base.h"

namespace license {
std::vector<uint8_t> unbase64(const std::string& base64_data);
std::string base64(const void* binaryData, size_t len, int lineLenght = -1);

namespace os {

// Base64 Encoding with OpenSSL
std::string base64_encode(const std::vector<uint8_t>& data);

// Base64 Decoding with OpenSSL
std::vector<uint8_t> base64_decode(const std::string& encoded_data);

} // namespace os
} /* namespace license */

#endif /* SRC_LIBRARY_OS_BASE64_HPP_ */
