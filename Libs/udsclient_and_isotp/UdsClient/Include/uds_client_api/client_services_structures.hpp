#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "common/uds_session.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/uds_return_types.hpp"

using dtc_mask = uint32_t;
using dtc_status = uint8_t;
using extended_data_record = std::map<std::string, double>;
using extended_data_for_dtc = std::map<uint8_t, extended_data_record>;
using dtc_and_status_record = std::map<dtc_mask, dtc_status>;
using dtc_snapshot_record_identification = uint8_t;
using freeze_frame_multiple_dids_data = std::map<uint16_t, std::vector<uint8_t>>;
using multiple_freeze_frames_for_dtc = std::map<uint8_t, freeze_frame_multiple_dids_data>;

namespace uds_client {
struct freeze_frames_for_dtc {
  multiple_freeze_frames_for_dtc freeze_frames;
};

struct extended_data_for_dtcs {
  std::map<dtc_mask, extended_data_record> data;
};
}  // namespace uds_client

namespace uds {

struct version_params {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
};

enum type_of_server { dynamic_library = 0, embedded_code = 1 };

struct DLL_PUBLIC dtc_data {
  uint8_t dtc_severity;
  uint8_t dtc_functional_unit;
  uint8_t dtc_status;

  dtc_data();
  dtc_data(uint8_t fst, uint8_t snd, uint8_t trd);

  bool operator==(const dtc_data &d1) const;
  bool operator!=(const dtc_data &d1) const;
  bool operator<(const dtc_data &d1) const;
  bool operator>(const dtc_data &d1) const;
};
}  // namespace uds

namespace uds_client {
struct flash_memory_sector {
  std::string name;
  uint32_t number;
  uint32_t address;
  uint32_t length;
};

struct ecu_memory_segment {
  std::vector<uint8_t> data;
  uint32_t start_address;
};

enum class programming_error {  // used to indicate some specific programming during the programming sequence
  checksum_error,
  erase_sector_error,
  session_change_error,
  generic_error,
  no_flash_sectors_given_error,
  internal_clear_sectors_not_given
};

struct uds_programming_result final {
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_programming_result(uds::response::positive_response _response);
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_programming_result(uds::response::negative_response error);
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_programming_result(programming_error error);
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_programming_result(uds::response::response_error error);
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_programming_result(std::variant<uds::response::positive_response, uds::response::negative_response,
                                      uds::response::response_error, programming_error> &&_response);

  const std::variant<uds::response::positive_response, uds::response::negative_response, uds::response::response_error,
                     programming_error>
      response;
};

enum class memory_type { internal, external };

std::string memory_type_as_string(memory_type type);

}  // namespace uds_client
