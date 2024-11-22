/*
 * ethernet_and_disk_strategy.cpp
 *
 *  Created on: Nov 20, 2024
 *      Author: DB
 */

#include <array>
#include <vector>
#include <cstring>
#include <algorithm>
//#include "../base/base64.h"
#include "../os/base64.hpp"

#include <licensecc/datatypes.h>
#include <licensecc_properties.h>
#include "../base/base.h"
#include "../os/network.hpp"
#include "hw_identifier.hpp"
#include "ethernet_and_disk_strategy.hpp"

namespace license {
namespace hw_identifier {
using namespace std;

EthernetAndDiskStrategy::EthernetAndDiskStrategy() {}

EthernetAndDiskStrategy::~EthernetAndDiskStrategy() {}

LCC_API_HW_IDENTIFICATION_STRATEGY EthernetAndDiskStrategy::identification_strategy() const {
	return STRATEGY_ETHERNET_AND_DISK;
}

FUNCTION_RETURN EthernetAndDiskStrategy::generate_pc_id(std::unique_ptr<HwIdentifier> &pc_id) const {
	const vector<unique_ptr<HwIdentifier>> available_ids = alternative_ids();
	FUNCTION_RETURN result = FUNC_RET_NOT_AVAIL;
	if (available_ids.size() > 0) {
                //std::cout << "ethernet_and_disk: available_ids[0]=" << available_ids[0]->print() << std::endl;
		pc_id = available_ids[0]->clone();
                //std::cout << "ethernet_and_disk: generated pc_id=" << pc_id->print() << std::endl;
		result = FUNC_RET_OK;
	}
	return result;
}

std::string print(std::vector<uint8_t> const& m_data) {
	string result = base64(m_data.data(), m_data.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
	return result.substr(0, result.size() - 1);
}

std::string print(std::array<uint8_t,HW_IDENTIFIER_PROPRIETARY_DATA_EXT> const& m_data) {
	string result = base64(m_data.data(), m_data.size(), 5);
	std::replace(result.begin(), result.end(), '\n', '-');
	return result.substr(0, result.size() - 1);
}

vector<unique_ptr<HwIdentifier>> EthernetAndDiskStrategy::alternative_ids() const {
	vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA_EXT>> data;
        unique_ptr<IdentificationStrategy> eth=IdentificationStrategy::get_strategy(STRATEGY_ETHERNET);
        unique_ptr<IdentificationStrategy> disk=IdentificationStrategy::get_strategy(STRATEGY_DISK);
        vector<unique_ptr<HwIdentifier>> eth_ids=eth->alternative_ids();
        vector<unique_ptr<HwIdentifier>> disk_ids=disk->alternative_ids();
	vector<unique_ptr<HwIdentifier>> identifiers;
	for (auto& it_eth: eth_ids) {
		vector<uint8_t> eth_data=it_eth->get_data();
		for (auto& it_disk: disk_ids) {
			auto pc_id = std::make_unique<HwIdentifier2>();
			pc_id->set_identification_strategy(identification_strategy());
			array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA_EXT> identifier = {};
			vector<uint8_t> disk_data=it_disk->get_data();
			std::copy(eth_data.begin()+2, eth_data.end(), identifier.begin()+1); // skip first 2 administrative bytes
			std::copy(disk_data.begin()+2, disk_data.end(), identifier.begin()+eth_data.size()-1); // skip first 2 administrative bytes
			pc_id->set_data(identifier);
/*
	                cout << "Input ethernet: " << print(eth_data) << endl;
	                cout << "Input disk: " << print(disk_data) << endl;
	                cout << "Merged id: " << print(identifier) << endl;
	                cout << "Generated ethernet+disk: " << pc_id->print() << endl;
*/
			identifiers.push_back(std::move(pc_id));
		}	
	}	
	return identifiers;
}

}  // namespace hw_identifier
} /* namespace license */
