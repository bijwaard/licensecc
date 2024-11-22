#include <array>
#include "identification_strategy.hpp"
#include "default_strategy.hpp"
#include "ethernet.hpp"
#include "disk_strategy.hpp"
#include "ethernet_and_disk_strategy.hpp"
namespace license {
namespace hw_identifier {

using namespace std;
LCC_EVENT_TYPE IdentificationStrategy::validate_identifier(const std::unique_ptr<HwIdentifier> &identifier) const {
	LCC_EVENT_TYPE result = IDENTIFIERS_MISMATCH;

	if (identifier->get_identification_strategy() == identification_strategy()) {
		const vector<unique_ptr<HwIdentifier>> available_ids = alternative_ids();
		for (const auto& it : available_ids) {
			if (it == identifier) {
				result = LICENSE_OK;
				break;
			}
		}
	}
	return result;
}

FUNCTION_RETURN IdentificationStrategy::generate_pc_id(std::unique_ptr<HwIdentifier> &pc_id) const {
	const vector<unique_ptr<HwIdentifier>> available_ids = alternative_ids();
	FUNCTION_RETURN result = FUNC_RET_NOT_AVAIL;
	if (available_ids.size() > 0) {
		pc_id = available_ids[0]->clone();
		result = FUNC_RET_OK;
	}
	return result;
}

std::unique_ptr<IdentificationStrategy> IdentificationStrategy::get_strategy(LCC_API_HW_IDENTIFICATION_STRATEGY strategy) {
	unique_ptr<IdentificationStrategy> result;
	switch (strategy) {
		case STRATEGY_DEFAULT:
			result = unique_ptr<IdentificationStrategy>(dynamic_cast<IdentificationStrategy*>(new DefaultStrategy()));
			break;
		case STRATEGY_ETHERNET:
			result = unique_ptr<IdentificationStrategy>(dynamic_cast<IdentificationStrategy*>(new Ethernet(false)));
			break;
		case STRATEGY_IP_ADDRESS:
			result = unique_ptr<IdentificationStrategy>(dynamic_cast<IdentificationStrategy*>(new Ethernet(true)));
			break;
		case STRATEGY_DISK:
			result = unique_ptr<IdentificationStrategy>(dynamic_cast<IdentificationStrategy*>(new DiskStrategy()));
			break;
		case STRATEGY_ETHERNET_AND_DISK:
			result = unique_ptr<IdentificationStrategy>(dynamic_cast<IdentificationStrategy*>(new EthernetAndDiskStrategy()));
			break;
		default:
			throw logic_error("strategy not supported");
	}
	return result;
}

std::unique_ptr<HwIdentifier> IdentificationStrategy::get_identifier(std::string const& param) {
	if (param.size()>=HW_IDENTIFIER_PROPRIETARY_DATA_EXT) {
        	return unique_ptr<HwIdentifier>(dynamic_cast<HwIdentifier*>(new HwIdentifier2(param)));
	} else {
        	return unique_ptr<HwIdentifier>(dynamic_cast<HwIdentifier*>(new HwIdentifier(param)));
	}
}

std::unique_ptr<HwIdentifier> IdentificationStrategy::get_identifier(LCC_API_HW_IDENTIFICATION_STRATEGY strategy) {
	unique_ptr<HwIdentifier> result;
	switch (strategy) {
		case STRATEGY_DEFAULT:
		case STRATEGY_ETHERNET:
		case STRATEGY_IP_ADDRESS:
		case STRATEGY_DISK:
			result = unique_ptr<HwIdentifier>(dynamic_cast<HwIdentifier*>(new HwIdentifier()));
			break;
		case STRATEGY_ETHERNET_AND_DISK:
			result = unique_ptr<HwIdentifier>(dynamic_cast<HwIdentifier*>(new HwIdentifier2()));
			break;
		default:
			throw logic_error("strategy not supported");
	}
	return result;
}

}  // namespace hw_identifier
}  // namespace license
