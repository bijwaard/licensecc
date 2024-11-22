/*
 * hw_identifier.cpp
 *
 *  Created on: Dec 22, 2019
 *      Author: GC
 */

#include <algorithm>
#include "hw_identifier.hpp"
//#include "../base/base64.h"
#include "../os/base64.hpp"

static std::string print(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>& data) {
	std::string result = license::base64(data.data(), data.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
	return result.substr(0, result.size() - 1);
}

static std::string print(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA_EXT>& data) {
	std::string result = license::base64(data.data(), data.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
	return result.substr(0, result.size() - 1);
}

namespace license {
namespace hw_identifier {

using namespace std;

HwIdentifier::HwIdentifier() {}

HwIdentifier::HwIdentifier(const std::string& param) {
	string tmp_str(param);  // throw away const
	std::replace(tmp_str.begin(), tmp_str.end(), '-', '\n');
	vector<uint8_t> decoded = unbase64(tmp_str);
	if (decoded.size() != HW_IDENTIFIER_PROPRIETARY_DATA + 1) {
		cerr << decoded.size();
		throw logic_error("wrong identifier size " + param + ", decoded=" + std::to_string(decoded.size()) + ", expected=" + std::to_string(HW_IDENTIFIER_PROPRIETARY_DATA + 1));
	}
	std::copy_n(decoded.begin(), HW_IDENTIFIER_PROPRIETARY_DATA + 1, m_data.begin());
}

HwIdentifier::~HwIdentifier() {}

HwIdentifier::HwIdentifier(const HwIdentifier& other) : m_data(other.m_data) {}

void HwIdentifier::set_identification_strategy(LCC_API_HW_IDENTIFICATION_STRATEGY strategy) {
	if (strategy == STRATEGY_NONE || strategy == STRATEGY_DEFAULT) {
		throw logic_error("Only known strategies are permitted");
	}
	uint8_t stratMov = (strategy << 5);
	m_data[1] = (m_data[1] & 0x1F) | stratMov;
}

void HwIdentifier::set_use_environment_var(bool use_env_var) {
	if (use_env_var) {
		m_data[0] = m_data[0] | 0x40;
	} else {
		m_data[0] = m_data[0] & ~0x40;
	}
}

void HwIdentifier::set_data(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>& data) {
        //std::cout << "HwIdentifier->set_data: " << ::print(data) << std::endl;
        //std::cout << "In_Byte: " ; for (size_t i = 0; i < data.size(); ++i) { std::cout << ", " << static_cast<int>(data[i]); } std::cout << std::endl;
	m_data[1] = (m_data[1] & (~0x1f)) | (data[0] & 0x1f);
	for (int i = 1; i < HW_IDENTIFIER_PROPRIETARY_DATA; i++) {
		m_data[i + 1] = data[i];
	}
        //std::cout << "OutByte: " ; for (size_t i = 0; i < m_data.size(); ++i) { std::cout << ", " << static_cast<int>(m_data[i]); } std::cout << std::endl;
        //std::cout << "HwIdentifier->set_data result: " << print() << std::endl;
}

std::string HwIdentifier::print() const {
	string result = base64(m_data.data(), m_data.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
	return result.substr(0, result.size() - 1);
}

vector<uint8_t> HwIdentifier::get_data() {
	vector<uint8_t> result(m_data.begin(),m_data.end());
        return result;
}

LCC_API_HW_IDENTIFICATION_STRATEGY HwIdentifier::get_identification_strategy() const {
	uint8_t stratMov = m_data[1] >> 5;
	return static_cast<LCC_API_HW_IDENTIFICATION_STRATEGY>(stratMov);
}

bool HwIdentifier::data_match(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>& data) const {
	bool equals = true;
        //std::cout << "HWIdenfifier:data_match self=" << print() << " with " << ::print(data) << std::endl;
	for (int i = 0; i < HW_IDENTIFIER_PROPRIETARY_DATA && equals; i++) {
		equals = (i == 0) ? ((data[i] & 0x1f) == (m_data[i + 1] & 0x1f)) : (data[i] == m_data[i + 1]);
	}
	return equals;
}

std::unique_ptr<HwIdentifier> HwIdentifier::clone() const {
	return std::make_unique<HwIdentifier>(*this);
}

bool HwIdentifier::equals(const HwIdentifier& other) const {
	bool equals = get_identification_strategy() == other.get_identification_strategy();
        //std::cout << "HWIdenfifier:equals " << print() << " with other=" << other.print() << std::endl;
	for (int i = 0; i < HW_IDENTIFIER_PROPRIETARY_DATA && equals; i++) {
		equals = (i == 0) ? ((m_data[i + 1] & 0x1f) == (other.m_data[i + 1] & 0x1f))
						  : (m_data[i + 1] == other.m_data[i + 1]);
	}
	return equals;
}

bool operator==(const std::unique_ptr<HwIdentifier> &lhs, const std::unique_ptr<HwIdentifier> &rhs) {
	if (!lhs || !rhs)
		return lhs.get() == rhs.get();
	return lhs->equals(*rhs);
}

HwIdentifier2::HwIdentifier2() {}

HwIdentifier2::HwIdentifier2(const std::string& param) {
	string tmp_str(param);  // throw away const
	std::replace(tmp_str.begin(), tmp_str.end(), '-', '\n');
	vector<uint8_t> decoded = unbase64(tmp_str);
	if (decoded.size() != HW_IDENTIFIER_PROPRIETARY_DATA_EXT + 1) {
		cerr << decoded.size();
		throw logic_error("wrong identifier size " + param + ", decoded=" + std::to_string(decoded.size()) + ", expected=" + std::to_string(HW_IDENTIFIER_PROPRIETARY_DATA + 1));
	}
	std::copy_n(decoded.begin(), HW_IDENTIFIER_PROPRIETARY_DATA_EXT + 1, m_data2.begin());
}
HwIdentifier2::HwIdentifier2(const HwIdentifier2& other) : m_data2(other.m_data2) {}

HwIdentifier2::~HwIdentifier2() {}

void HwIdentifier2::set_identification_strategy(LCC_API_HW_IDENTIFICATION_STRATEGY strategy) {
	if (strategy == STRATEGY_NONE || strategy == STRATEGY_DEFAULT) {
		throw logic_error("Only known strategies are permitted");
	}
	uint8_t stratMov = (strategy << 5);
        //std::cout << "HwIdentifier2->set_identification_strategy: strategy=" << strategy << ", byte: " << int(stratMov) << ", orig[1]=" << m_data2[1] << std::endl;
	m_data2[1] = (m_data2[1] & 0x1F) | stratMov;
        //std::cout << "OutByte: " ; for (size_t i = 0; i < m_data2.size(); ++i) { std::cout << ", " << static_cast<int>(m_data2[i]); } std::cout << std::endl;
        //std::cout << "HwIdentifier2->set_identification_strategy result: " << print() << std::endl;
}

void HwIdentifier2::set_use_environment_var(bool use_env_var) {
	if (use_env_var) {
		m_data2[0] = m_data2[0] | 0x40;
	} else {
		m_data2[0] = m_data2[0] & ~0x40;
	}
}

void HwIdentifier2::set_data(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA_EXT>& data) {
        //std::cout << "HwIdentifier2->set_data: " << ::print(data) << std::endl;
        //std::cout << "In_Byte: " ; for (size_t i = 0; i < data.size(); ++i) { std::cout << ", " << static_cast<int>(data[i]); } std::cout << std::endl;
	m_data2[1] = (m_data2[1] & (~0x1f)) | (data[0] & 0x1f);
	for (int i = 1; i < HW_IDENTIFIER_PROPRIETARY_DATA_EXT; i++) {
		m_data2[i + 1] = data[i];
	}
        //std::cout << "OutByte: " ; for (size_t i = 0; i < m_data2.size(); ++i) { std::cout << ", " << static_cast<int>(m_data2[i]); } std::cout << std::endl;
        //std::cout << "HwIdentifier2->set_data result: " << print() << std::endl;
}

std::unique_ptr<HwIdentifier> HwIdentifier2::clone() const {
	return std::make_unique<HwIdentifier2>(*this);
}

std::string HwIdentifier2::print() const {
        //std::cout << "HwIdentifier2->print(): input size=" << m_data2.size() << std::endl;
	string result = base64(m_data2.data(), m_data2.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
        //std::cout << "HwIdentifier2->print(): result size=" << result.size() << std::endl;
	return result.substr(0, result.size() - 1);
}

LCC_API_HW_IDENTIFICATION_STRATEGY HwIdentifier2::get_identification_strategy() const {
	uint8_t stratMov = m_data2[1] >> 5;
	return static_cast<LCC_API_HW_IDENTIFICATION_STRATEGY>(stratMov);
}

bool HwIdentifier2::data_match(const std::array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA_EXT>& data) const {
	bool equals = true;
        //std::cout << "HWIdenfifier2:data_match self=" << print() << " with " << ::print(data) << std::endl;
	for (int i = 0; i < HW_IDENTIFIER_PROPRIETARY_DATA_EXT && equals; i++) {
		equals = (i == 0) ? ((data[i] & 0x1f) == (m_data2[i + 1] & 0x1f)) : (data[i] == m_data2[i + 1]);
	}
	return equals;
}

bool HwIdentifier2::equals(const HwIdentifier& other) const {
	const HwIdentifier2* other2 = dynamic_cast<const HwIdentifier2*>(&other);
	bool equals = get_identification_strategy() == other2->get_identification_strategy();
        //std::cout << "HWIdenfifier2:equals " << print() << " with other=" << other2->print() << std::endl;
	for (int i = 0; i < HW_IDENTIFIER_PROPRIETARY_DATA_EXT && equals; i++) {
		equals = (i == 0) ? ((m_data2[i + 1] & 0x1f) == (other2->m_data2[i + 1] & 0x1f))
						  : (m_data2[i + 1] == other2->m_data2[i + 1]);
	}
	return equals;
}

}  // namespace hw_identifier
} /* namespace license */
