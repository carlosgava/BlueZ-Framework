//
//  BLEInternal.h
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 28/07/25.
//

#pragma once

#include "BLEControllerTypes.h"
#include <set>

namespace OSXBLE {
    class OSXBLEInternal {
    private:
        CallbackHolder callback_holder;
        std::set<OSXBLE::BluetoothAddress> detected_addresses;
    public:
        OSXBLEInternal();
        ~OSXBLEInternal();

        void scan_start();
        void scan_stop();
        void disconnect();
        void dispose();
        
        void setup(CallbackHolder callback_holder);
        void scan_timeout(int32_t timeout_ms);
        void connect(const BluetoothAddress &address);
        void write_request(BluetoothUUID service, BluetoothUUID characteristic, DataChunk &data);
        void write_command(BluetoothUUID service, BluetoothUUID characteristic, DataChunk &data);
        void read(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_read);
        void notify(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_notify);
        void indicate(BluetoothUUID service, BluetoothUUID characteristic,
        std::function<void(const uint8_t *data, uint32_t length)> callback_on_indicate);
        void unsubscribe(BluetoothUUID service, BluetoothUUID characteristic);
        
        bool is_connected();
        bool scan_is_active();
    };
}
