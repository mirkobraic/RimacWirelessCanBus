#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "isotp_api/can/isotp_can_provider.hpp"
#include "isotp_api/export_macro.hpp"
#include "isotp_api/isotp_message.hpp"
#include "isotp_api/logger.hpp"

struct isotp_provider_handle;
typedef struct isotp_provider_handle isotp_provider_handle;
extern "C" DLL_PUBLIC isotp_provider_handle *construct_isotp_provider_handle(
    std::shared_ptr<isotp::can_provider> provider, std::vector<std::pair<uint32_t, uint32_t>> rx_tx_pairs,
    std::shared_ptr<logger> logger);
extern "C" DLL_PUBLIC void destruct_isotp_provider_handle(isotp_provider_handle *handle);
extern "C" DLL_PUBLIC void send_isotp_message_over_provider_handle(isotp_provider_handle *handle, isotp_message *msg);
extern "C" DLL_PUBLIC void set_received_isotp_message_callback_for_provider_handle(
    isotp_provider_handle *handle, std::function<void(isotp_message *)>);
extern "C" DLL_PUBLIC void stop_processing_received_can_messages_for_rx(isotp_provider_handle *handle, uint32_t rx);
extern "C" DLL_PUBLIC void stop_processing_received_can_messages_for_all_rx(isotp_provider_handle *handle);
extern "C" DLL_PUBLIC void start_processing_received_can_messages_for_rx(isotp_provider_handle *handle, uint32_t rx);
extern "C" DLL_PUBLIC void start_processing_received_can_messages_for_all_rx(isotp_provider_handle *handle);