#pragma once
#include <stdint.h>
#include <functional>
#include <variant>
#include <vector>
#include "common/overload_template_mp.hpp"

namespace uds {
namespace response {

enum class positive_response {};  // used to indicate empty positive response

enum class negative_response {
  general_reject,                                  // 0x10
  service_not_supported,                           // 0x11
  subfunction_not_supported,                       // 0x12
  incorrect_message_length_or_invalid_format,      // 0x13
  response_too_long,                               // 0x14
  busy_repeat_request,                             // 0x21
  conditions_not_correct,                          // 0x22
  request_sequence_error,                          // 0x24
  no_response_from_subnet_component,               // 0x25
  failure_prevents_execution_of_requested_action,  // 0x26
  request_out_of_range,                            // 0x31
  security_access_denied,                          // 0x33
  invalid_key,                                     // 0x35
  exceed_number_of_attempts,                       // 0x36
  required_time_delay_not_expired,                 // 0x37
  upload_download_not_accepted,                    // 0x70
  transfer_data_suspended,                         // 0x71
  general_programming_failure,                     // 0x72
  wrong_block_sequence_counter,                    // 0x73
  subfunction_not_supported_in_active_session,     // 0x7E
  service_not_supported_in_active_session,         // 0x7F
  rpm_too_high,                                    // 0x81
  rpm_too_low,                                     // 0x82
  engine_is_running,                               // 0x83
  engine_is_not_running,                           // 0x84
  engine_run_time_too_low,                         // 0x85
  temperature_too_high,                            // 0x86
  temperature_too_low,                             // 0x87
  vehicle_speed_too_high,                          // 0x88
  vehicle_speed_too_low,                           // 0x89
  throttle_pedal_too_high,                         // 0x8A
  throttle_pedal_too_low,                          // 0x8B
  transmission_range_not_in_neutral,               // 0x8C
  transmission_range_not_in_gear,                  // 0x8D
  break_switch_not_closed,                         // 0x8F
  shifter_lever_not_in_park,                       // 0x90
  torque_converter_clutch_locker,                  // 0x91
  voltage_too_high,                                // 0x92
  voltage_too_low,                                 // 0x93
  unsupported_error_code
};

enum class response_error {  // used to indicate some problem with communication with server
  invalid_response_length,
  no_response,
  response_parse_error,
  generic_error,
  unsupported_client_id,
  request_already_in_process
};

// we place the implementation here because this file will be a part of the API
template <typename T>
struct uds_client_response final {
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_client_response(const T&& _response) : response{_response} {}
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_client_response(uds::response::negative_response error) : response{error} {}
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_client_response(uds::response::response_error error) : response{error} {}
  // cppcheck-suppress noExplicitConstructor ; implicit calling is wanted in this case
  uds_client_response(std::variant<T, uds::response::negative_response, uds::response::response_error>&& _response)
      : response{_response} {}
  operator bool() const {
    auto res = response;
    return std::visit(overload{[](T& unwrapped_result) -> bool { return true; },
                               [](uds::response::negative_response& error) -> bool { return false; },
                               [](uds::response::response_error& error) -> bool { return false; }},
                      res);
  }

  template <typename U>
  U unpack_response(std::function<U(const T&)> if_response,
                    std::function<U(const uds::response::negative_response&)> if_negative_response,
                    std::function<U(const uds::response::response_error&)> if_response_error) const {
    auto res = response;
    return std::visit(overload{if_response, if_negative_response, if_response_error}, res);
  }

  void unpack_response(std::function<void(const T&)> if_response,
                       std::function<void(const uds::response::negative_response&)> if_negative_response,
                       std::function<void(const uds::response::response_error&)> if_response_error) const {
    auto res = response;
    std::visit(overload{if_response, if_negative_response, if_response_error}, res);
  }

  const std::variant<T, uds::response::negative_response, uds::response::response_error> response;
};
}  // namespace response
}  // namespace uds
