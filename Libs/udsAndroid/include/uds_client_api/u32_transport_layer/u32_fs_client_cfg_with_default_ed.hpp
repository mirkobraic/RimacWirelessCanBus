#pragma once
#include <memory>
#include "common/logger.hpp"
#include "uds_client_api/client_configuration_manager.hpp"
#include "uds_client_api/export_macro.hpp"

namespace uds {
namespace client {
struct u32_client_configuration_handle {};

namespace configuration {
class u32_fs_client_cfg_with_default_ed final : public client_configuration_manager<uint32_t> {
 public:
  /**
   * @brief Filesystem configration which expects there to be an environment variable UDS_CLIENT_CONFIGURATION_ROOT
   * containing wanted configurations. Consult the UDS Client integration document for more details
   *
   * @throw uds::exceptions::missing_uds_environment_variable_exception if UDS_CLIENT_CONFIGURATION_ROOT environment
   * variable is not set
   * @param logger logging provider
   */
  DLL_PUBLIC explicit u32_fs_client_cfg_with_default_ed(std::shared_ptr<logger> logger);

  /**
   * @brief Filesystem configration which will search for files under specific path in the given configuration root.
   * Consult the UDS Client integration document for more details
   *
   * @param root root of the configuration files
   * @param logger logging provider
   */
  DLL_PUBLIC u32_fs_client_cfg_with_default_ed(std::string& root, std::shared_ptr<logger> logger);

  DLL_PUBLIC virtual ~u32_fs_client_cfg_with_default_ed();

  DLL_PUBLIC std::vector<std::pair<uint32_t, uint32_t>> get_rx_tx_pairs() override;
  DLL_PUBLIC std::map<std::pair<uint32_t, uint32_t>, std::map<uint16_t, uint16_t>> get_freeze_frame_read_instructions()
      override;
  DLL_PUBLIC std::map<std::pair<uint32_t, uint32_t>, extended_data_record_id_to_length>
  get_extended_data_read_instructions() override;
  DLL_PUBLIC std::map<std::pair<uint32_t, uint32_t>, std::map<uint16_t, data_reader_instruction_for_single_did>>
  get_data_identifier_read_instructions() override;
  DLL_PUBLIC uint8_t get_threadpool_size() override;

 private:
  u32_client_configuration_handle* configuration_handle;
};
}  // namespace configuration
}  // namespace client
}  // namespace uds
