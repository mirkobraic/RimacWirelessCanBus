#include "UdsConstantsUnpacker.h"

UdsConstantsUnpacker::UdsConstantsUnpacker()
{ }

QString UdsConstantsUnpacker::unpackNegativeResponse(uds::response::negative_response res)
{
    switch (res) {
    case uds::response::negative_response::general_reject:                                  return "General reject";
    case uds::response::negative_response::service_not_supported:                           return "Service not supported";
    case uds::response::negative_response::subfunction_not_supported:                       return "Subfunction not supported";
    case uds::response::negative_response::incorrect_message_length_or_invalid_format:      return "Incorrect message length or invalid format";
    case uds::response::negative_response::response_too_long:                               return "Response too long";
    case uds::response::negative_response::busy_repeat_request:                             return "Busy repeat request";
    case uds::response::negative_response::conditions_not_correct:                          return "Conditions not correct";
    case uds::response::negative_response::request_sequence_error:                          return "Request sequence error";
    case uds::response::negative_response::no_response_from_subnet_component:               return "No response from subnet component";
    case uds::response::negative_response::failure_prevents_execution_of_requested_action:  return "Failure prevents execution of requested action";
    case uds::response::negative_response::request_out_of_range:                            return "Request out of range";
    case uds::response::negative_response::security_access_denied:                          return "Security access denied";
    case uds::response::negative_response::invalid_key:                                     return "Invalid key";
    case uds::response::negative_response::exceed_number_of_attempts:                       return "Exceed number of attempts";
    case uds::response::negative_response::required_time_delay_not_expired:                 return "Required time delay not expired";
    case uds::response::negative_response::upload_download_not_accepted:                    return "Upload or download not accepted";
    case uds::response::negative_response::transfer_data_suspended:                         return "Transfer data suspended";
    case uds::response::negative_response::general_programming_failure:                     return "General programming failure";
    case uds::response::negative_response::wrong_block_sequence_counter:                    return "Wrong block sequence counter";
    case uds::response::negative_response::subfunction_not_supported_in_active_session:     return "Subfunction not supported in active session";
    case uds::response::negative_response::service_not_supported_in_active_session:         return "Service not supported in active session";
    case uds::response::negative_response::rpm_too_high:                                    return "RPM too high";
    case uds::response::negative_response::rpm_too_low:                                     return "RPM too low";
    case uds::response::negative_response::engine_is_running:                               return "Engine is running";
    case uds::response::negative_response::engine_is_not_running:                           return "Engine is not running";
    case uds::response::negative_response::engine_run_time_too_low:                         return "Engine run time too low";
    case uds::response::negative_response::temperature_too_high:                            return "Temperature too high";
    case uds::response::negative_response::temperature_too_low:                             return "Temperature too low";
    case uds::response::negative_response::vehicle_speed_too_high:                          return "Vehicle speed too high";
    case uds::response::negative_response::vehicle_speed_too_low:                           return "Vheicle speed too low";
    case uds::response::negative_response::throttle_pedal_too_high:                         return "Throttle pedal too high";
    case uds::response::negative_response::throttle_pedal_too_low:                          return "Throttle pedal too low";
    case uds::response::negative_response::transmission_range_not_in_neutral:               return "Transmission range not in neutral";
    case uds::response::negative_response::transmission_range_not_in_gear:                  return "Transmission range not in gear";
    case uds::response::negative_response::break_switch_not_closed:                         return "Break switch not closed";
    case uds::response::negative_response::shifter_lever_not_in_park:                       return "Shifter lever not in park";
    case uds::response::negative_response::torque_converter_clutch_locker:                  return "Torque converter clutch locker";
    case uds::response::negative_response::voltage_too_high:                                return "Voltage too high";
    case uds::response::negative_response::voltage_too_low:                                 return "Voltage too low";
    case uds::response::negative_response::unsupported_error_code:                          return "Unsupported error code";
    default:                                                                                return "Unknown error";
    }
}

QString UdsConstantsUnpacker::unpackResponseError(uds::response::response_error res)
{
    switch (res) {
    case uds::response::response_error::invalid_response_length:    return "Invalid response length";
    case uds::response::response_error::no_response:                return "No response";
    case uds::response::response_error::response_parse_error:       return "Response parse error";
    case uds::response::response_error::generic_error:              return "Generic error";
    case uds::response::response_error::unsupported_client_id:      return "Unsuported client id";
    default:                                                        return "Unknown error";
    }
}
