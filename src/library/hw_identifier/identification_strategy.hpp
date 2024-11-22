/**
 * \file identification_strategy.hpp
 *
 *  Created on: Jan 1, 2020
 *      Author: devel
 */

#ifndef SRC_LIBRARY_PC_IDENTIFIER_IDENTIFICATION_STRATEGY_HPP_
#define SRC_LIBRARY_PC_IDENTIFIER_IDENTIFICATION_STRATEGY_HPP_

#include <licensecc/datatypes.h>
#include <licensecc_properties.h>
#include <vector>
#include <memory>
#include "../base/base.h"
#include "hw_identifier.hpp"

namespace license {
namespace hw_identifier {

/**
 * Abstract class that represent a way to calculate hardware identifiers.
 */
class IdentificationStrategy {
protected:
	IdentificationStrategy(){};

public:
	virtual ~IdentificationStrategy(){};
	virtual LCC_API_HW_IDENTIFICATION_STRATEGY identification_strategy() const = 0;
	virtual FUNCTION_RETURN generate_pc_id(std::unique_ptr<HwIdentifier> &identifier_out) const;
	virtual std::vector<std::unique_ptr<HwIdentifier>> alternative_ids() const = 0;
	virtual LCC_EVENT_TYPE validate_identifier(const std::unique_ptr<HwIdentifier> &identifier_in) const;

	/**
	 * Factory method to create an instance of IdentificationStrategy
	 * @param strategy
	 * @return
	 */
	static std::unique_ptr<IdentificationStrategy> get_strategy(LCC_API_HW_IDENTIFICATION_STRATEGY strategy);
	static std::unique_ptr<HwIdentifier> get_identifier(LCC_API_HW_IDENTIFICATION_STRATEGY strategy);
	static std::unique_ptr<HwIdentifier> get_identifier(std::string const& param);
};

}  // namespace hw_identifier
} /* namespace license */

#endif /* SRC_LIBRARY_PC_IDENTIFIER_IDENTIFICATION_STRATEGY_HPP_ */
