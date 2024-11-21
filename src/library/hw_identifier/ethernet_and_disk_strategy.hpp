/*
 * ethernet_and_disk_strategy.hpp
 *
 *  Created on: Nov 20, 2024
 *      Author: DB
 *      Author: devel
 */

#ifndef SRC_LIBRARY_PC_IDENTIFIER_ETHERNET_AND_DISK_HPP_
#define SRC_LIBRARY_PC_IDENTIFIER_ETHERNET_AND_DISK_HPP_

#include <memory>
#include "identification_strategy.hpp"
#include "ethernet.hpp"
#include "disk_strategy.hpp"

namespace license {
namespace hw_identifier {

class EthernetAndDiskStrategy : public IdentificationStrategy {
private:
public:
	EthernetAndDiskStrategy();
	virtual ~EthernetAndDiskStrategy();
        virtual FUNCTION_RETURN generate_pc_id(std::unique_ptr<HwIdentifier> &pc_id) const override;
	virtual std::vector<std::unique_ptr<HwIdentifier>> alternative_ids() const;
	virtual LCC_API_HW_IDENTIFICATION_STRATEGY identification_strategy() const override;
};

}  // namespace hw_identifier
} /* namespace license */

#endif /* SRC_LIBRARY_PC_IDENTIFIER_ETHERNET_AND_DISK_HPP_ */
