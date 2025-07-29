//
//  BLEController.h
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 28/07/25.
//

#ifndef BLEController_h
#define BLEController_h

#pragma once

#include "BLEControllerTypes.h"
#include "BLEController.h"

using namespace std::string_literals;

namespace OSXBLE {
    class OSXBLEInternal;

    class OSXBLEController {
    private:
        OSXBLEInternal *internal;

    public:
        OSXBLEController();
        ~OSXBLEController();

        void setup(CallbackHolder callback_holder);
        void scan_start();
        void scan_stop();
        void scan_timeout(int32_t timeout_ms);
        void connect(const BluetoothAddress &address);
        void write_request(BluetoothUUID service, BluetoothUUID characteristic, DataChunk data);
        void write_command(BluetoothUUID service, BluetoothUUID characteristic, DataChunk data);
        void read(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_read);
        void notify(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_notify);
        void indicate(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_indicate);
        void unsubscribe(BluetoothUUID service, BluetoothUUID characteristic);
        void disconnect();
        void dispose();
        bool scan_is_active();
        bool is_connected();
    };
}
#endif
