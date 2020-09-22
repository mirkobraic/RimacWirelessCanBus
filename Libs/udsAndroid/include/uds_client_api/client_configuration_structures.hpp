#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace uds {
namespace client {
namespace configuration {
struct data_reader_instruction_for_single_did {
  uint32_t length;
};

struct extended_data_record_id_to_length {
  std::map<uint8_t, uint16_t> extended_data_record_id_to_length_mapper;
};

template <typename T>
struct uds_client_configuration {
  T rx;
  T tx;
  std::map<uint16_t, uint16_t> freeze_frame_read_instructions;  // did to size mapper
  extended_data_record_id_to_length extended_data_read_instructions;
  std::map<uint16_t, data_reader_instruction_for_single_did> data_identifier_read_instructions;
};
}  // namespace configuration
}  // namespace client
}  // namespace uds
