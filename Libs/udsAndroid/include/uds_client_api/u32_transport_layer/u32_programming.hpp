#pragma once

#include <functional>
#include <optional>
#include <utility>
#include "uds_client_api/client_services_structures.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_programming final {
 public:
  explicit u32_programming(u32_uds_client_provider_handle* handle);

  /**
   * @brief Combines several requests and performs programming on a server ECU
   *
   * @param tx address of the server
   * @param clear_sectors sectors to be cleared with routine control
   * @param flash_segments sectors to be written to
   * @param switch_to_bootloader_sleep_period_in_seconds sleep period in secods before switching to bootloader
   * @param routine_control_calls_timeout_millis timout for routine control calls (when clearing and performing
   * checksum)
   * @param overall_progress_callback Callback to be called with active percentage of overall progress
   * @param per_progress_callback Callback to be called with updated active percentage of each phase
   * @param progress_phase_callback Callback to be called when entering new phase (callback is provided with the phase
   * name)
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds_programming_result program_flash_memory(
      uint32_t tx, const std::vector<flash_memory_sector>& clear_sectors,
      const std::vector<ecu_memory_segment>& flash_segments, std::function<void(float)>& overall_progress_callback,
      std::function<void(float)>& per_phase_progress_callback,
      std::function<void(const std::string&)>& progress_phase_callback,
      uint8_t switch_to_bootloader_sleep_period_in_seconds = 3, uint32_t routine_control_calls_timeout_millis = 5000);

  /**
   * @brief Combines serveral requests and performs programming on a server ECU (both external and internal flash)
   *
   * @param tx address of the server
   * @param clear_sectors (internal) sectors to be cleared with routine control (can be none if only external sectors
   * are to be written to)
   * @param internal_flash_segments internal sectors to be written to (can be none if only external sectors are to be
   * written to)
   * @param external_flash_segments external sectors to be written to (can be none if only internal sectors are to be
   * written to)
   * @param switch_to_bootloader_sleep_period_in_seconds sleep period in secods before switching to bootloader
   * @param internal_routine_control_calls_timeout_millis timout for routine control calls related to internal memory
   * (when clearing internal memroy and performing checksum for internal flash)
   * @param external_routine_control_calls_timeout_millis timout for routine control calls related to external memory
   * (when performing checksum for external flash)
   * @param overall_progress_callback Callback to be called with active percentage of overall progress
   * @param per_progress_callback Callback to be called with updated active percentage of each phase
   * @param progress_phase_callback Callback to be called when entering new phase (callback is provided with the phase
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds_programming_result program_flash_memory(
      uint32_t tx, const std::optional<std::vector<flash_memory_sector>>& clear_sectors_opt,
      const std::optional<std::vector<ecu_memory_segment>>& internal_flash_segments,
      const std::optional<std::vector<ecu_memory_segment>>& external_flash_segments,
      std::function<void(float)>& overall_progress_callback, std::function<void(float)>& progress_callback,
      std::function<void(const std::string&)>& progress_phase_callback,
      uint8_t switch_to_bootloader_sleep_period_in_seconds = 3,
      uint32_t internal_routine_control_calls_timeout_millis = 5000,
      uint32_t external_routine_control_calls_timeout_millis = 5000);

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client