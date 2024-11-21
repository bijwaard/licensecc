#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <boost/algorithm/string.hpp>
#include <iostream>

#include "../base64.hpp"

namespace license {
namespace os {

std::string base64_encode(const std::vector<uint8_t>& data) {
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newlines
	BIO_write(bio, data.data(), data.size());
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);

	std::string encoded_data(bufferPtr->data, bufferPtr->length);
	BIO_free_all(bio);

	return encoded_data;
}

std::vector<uint8_t> base64_decode(const std::string& encoded_data) {
	BIO *bio, *b64;
	int decodeLen = encoded_data.size();
	std::vector<uint8_t> decoded_data(decodeLen);
        //std::cout << "base64_decode(\"" << encoded_data << "\")" << std::endl;

	bio = BIO_new_mem_buf(encoded_data.data(), encoded_data.size());
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	//BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newlines
	decodeLen = BIO_read(bio, decoded_data.data(), encoded_data.size());
	decoded_data.resize(decodeLen);
	BIO_free_all(bio);

	return decoded_data;
}


} // namespace os

std::string base64(const void* binaryData, size_t len, int lineLenght) {
	std::vector<uint8_t> data((char*)binaryData,(char*)binaryData+len);
	std::string base64=os::base64_encode(data);
	std::string formatted;
	for (size_t i = 0; i < base64.size(); i += 4) {
		if (i > 0) {
			formatted += '-';
		}
		formatted += base64.substr(i, 4);
	}
	// Add padding when length is not multiple of 5 (note that \0 at end is also counted)
	while (formatted.size() % 5 != 0) {
		formatted += '=';
	}
	return formatted;
}

std::vector<uint8_t> unbase64(const std::string& base64_data) {
        //std::cout << "unbase64(\"" << base64_data << "\")" << std::endl;
	std::string cleaned(base64_data);
        //std::cout << "initial cleaned=\"" << cleaned << "\")" << std::endl;
        boost::erase_all(cleaned,"-");
        //boost::erase_all(cleaned,"="); // padding char should be ignored by decode
        //std::cout << "resulting cleaned=\"" << cleaned << "\")" << std::endl;
	return os::base64_decode(cleaned);
}

} /* namespace license */
