#pragma once

#include <map>
#include <optional>
#include "uds_client_api/client_services_structures.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_dtc_api final {
 public:
  explicit u32_dtc_api(u32_uds_client_provider_handle *handle);

  /**
   * @brief Request 0x19, Subfunction 0x1
   * Gets number of DTCs matching a status mask
   *
   * @param tx address of the server
   * @param mask Wanted mask to match on
   * @return response containing number of DTCs matching a mask if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<int> get_number_of_dtcs_matching_a_status_mask(
      uint32_t tx,
      uint8_t mask);  // read dtc, sub 0x1

  /**
   * @brief Request 0x19, Subfunction 0x02
   * Gets DTCs matching a status mask
   *
   * @param tx address of the server
   * @param mask wanted mask to match on
   * @return response containing DTCs matching a mask if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC
  uds::response::uds_client_response<std::map<dtc_mask, dtc_status>> get_dtcs_matching_a_status_mask(
      uint32_t tx, uint8_t mask);  // read dtc, sub 0x2

  /**
   * @brief Request 0x19, Subfunction 0x03
   * Gets IDs of snapshots stored on the server for all DTCs
   *
   * @param tx address of the server
   * @return response containing snapshot ids for all DTCs if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<uint32_t, std::vector<uint8_t>>> get_snapshot_ids_for_all_dtcs(
      uint32_t tx);  // read dtc, sub 0x3

  /**
   * @brief Request 0x19, Subfunction 0x04
   *
   * @param tx address of the server
   * @param dtc DTC for which snapshot needs to be retrieved
   * @param id id of the snapshot to be retrieved
   * @throw uds::exceptions::uds_invalid_argument if given dtc is invalid
   * @return response containing wanted snapshot if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC
  uds::response::uds_client_response<freeze_frame_multiple_dids_data> get_specific_snapshot_for_dtc(
      uint32_t tx, uint32_t dtc,
      uint8_t id);  // read dtc, sub 0x4, upper. we removed one map because only one elem

  /**
   * @brief Request 0x19, Subfunction 0x04, ID hardcoded 0xFF
   * Gets all snapshot for DTC

   * @param tx address of the server
   * @param dtc DTC for which snapshots need to be retrieved
   * @throw uds::exceptions::uds_invalid_argument if given dtc is invalid
   * @return response containing wanted snapshot if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<freeze_frames_for_dtc> get_all_snapshots_for_dtc(
      uint32_t tx, uint32_t dtc);  // read dtc, sub 0x4, lower

  /**
   * @brief Request 0x19, Subfunction 0x06
   * Gets extended data for DTC
   *
   * @param tx address of the server
   * @param dtc DTC for which extended data needs to be retrieved
   * @param id ID of the extended data to be retrieved
   * @throw uds::exceptions::uds_invalid_argument if given dtc is invalid
   * @return response containing extended data if request was successful, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<uint8_t, std::map<std::string, double>>>
  get_extended_data_for_dtc(uint32_t tx, uint32_t dtc,
                            uint8_t id);  // read dtc, sub 0x6

  /**
   * @brief Request 0x19, Subfunction 0x07
   * Gets number of DTCs matchibg given severity and status mask
   *
   * @param tx address of the server
   * @param severity_mask severity mask to be mached on by the server
   * @param status_mask status mask to be mached on by the server
   * @return response containing number of dtcs matching a severity and status mask if request was successful, specific
   * error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<int> get_number_of_dtcs_matching_severity_mask_and_dtc_status(
      uint32_t tx, uint8_t severity_mask,
      dtc_status status_mask);  // read dtc, sub 0x7

  /**
   * @brief Request 0x19, Subfunction 0x08
   * Gets severity and functional information for DTCs matchibg seveiry mask and dtc status
   *
   * @param tx address of the server
   * @param severity_mask severity mask to be matched on
   * @param status_mask status mask to be matched on
   * @return response containing severity and functional info for matched DTCs if request was successful, specific error
   * or negative response otherwise
   */
  DLL_PUBLIC
  uds::response::uds_client_response<std::map<uint32_t, uds::dtc_data>>
  get_severity_and_functional_information_for_dtcs_matching_severity_mask_and_dtc_status(
      uint32_t tx, uint8_t severity_mask, dtc_status status_mask);  // read dtc, sub 0x8

  /**
   * @brief Request 0x19, Subfunction 0x09
   * Gets severity and functional info for DTCs matching status mask
   *
   * @param tx address of the server
   * @param dtc dtc mask for matching
   * @throw uds::exceptions::uds_invalid_argument if given dtc is invalid
   * @return response containing severity and functional info if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<uint32_t, uds::dtc_data>>
  get_severity_and_functional_information_matching_dtc_status_mask(uint32_t tx,
                                                                   dtc_mask dtc);  // read dtc, sub 0x9

  /**
   * @brief Request 0x19, Subfunction 0x0A
   * Gets status for every supported DTC
   *
   * @param tx address of the server
   * @return response containing status of every DTC if request was successful, specific error or negative response
   * otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<dtc_mask, dtc_status>> get_status_of_all_supported_dtcs(
      uint32_t tx);  // read dtc, sub 0xa

  /**
   * @brief Request 0x19, Subfunction 0x0B
   * Gets first failed DTC and its status
   *
   * @param tx address of the server
   * @return response containing first failed DTC and its status if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::optional<std::pair<dtc_mask, dtc_status>>> get_first_failed_dtc(
      uint32_t tx);  // read dtc, sub 0xb

  /**
   * @brief Request 0x19, Subfunction 0x0C
   * Gets first comfirmed DTC and its status
   *
   * @param tx address of the server
   * @return response containing first confirmed DTC and its status if request was successful, specific error or
   * negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::optional<std::pair<dtc_mask, dtc_status>>> get_first_confirmed_dtc(
      uint32_t tx);  // read dtc, sub 0xc

  /**
   * @brief Request 0x19, Subfunction 0x0D
   * Gets most recent failed DTC and its status
   *
   * @param tx address of the server
   * @return response containing most recent failed DTC and its status if request was successful, specific error or
   * negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::optional<std::pair<dtc_mask, dtc_status>>>
  get_most_recent_failed_dtc(uint32_t tx);  // read dtc, sub 0xd

  /**
   * @brief Request 0x19, Subfunction 0x0E
   * Gets most recent confirmed DTC and its status
   *
   * @param tx address of the server
   * @return response containing most recent DTC and its status if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::optional<std::pair<dtc_mask, dtc_status>>>
  get_most_recent_confirmed_dtc(uint32_t tx);  // read dtc, sub 0xe

  /**
   * @brief Request 0x19, Subfunction 0x14
   * Gets prefailed DTCs
   *
   * @param tx address of the server
   * @return response containing prefailed DTCs if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<dtc_mask, dtc_status>> get_prefailed_dtcs(
      uint32_t tx);  // read dtc, sub 0x14

  /**
   * @brief Request 0x19, Subfunction 0x16
   * Gets all extended data for rectord number
   *
   * @param tx address of the server
   * @param extended_data_record_number record number of extended data for which records are to be retrieved
   * @return response containing all extended data for record number if request was successful, specific error or
   * negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<extended_data_for_dtcs> get_all_extended_data_for_record_number(
      uint32_t tx,
      uint8_t extended_data_record_number);  // read dtc, sub 0x16 + ignore uint8_t returned by low level api

  /**
   * @brief Request 0x14, Subfunction N/A
   * Requests to clear DTCs matching specified DTC mask
   *
   * @param tx address of the server
   * @param dtc dtc mask to be mached for clearing
   * @throw uds::exceptions::uds_invalid_argument if given dtc is invalid
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_clear_dtc_information(
      uint32_t tx, uint32_t dtc);

  /**
   * @brief Request 0x85, Subfunction 0x02
   * Requests to stop updating status bits for all DTCs
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> stop_updating_all_dtc_status_bits(
      uint32_t tx);  // control_dtc_setting

  /**
   * @brief Request 0x85, Subfunction 0x01
   * Requests to start updating status bits for all DTCs
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> start_updating_all_dtc_status_bits(
      uint32_t tx);  // control_dtc_setting

 private:
  u32_uds_client_provider_handle *uds_client_handle;
};
}  // namespace uds_client