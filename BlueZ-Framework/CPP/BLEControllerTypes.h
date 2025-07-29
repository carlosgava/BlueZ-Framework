//
//  BLEControllerTypes.h
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 28/07/25.
//

#ifndef BLEControllerTypes_h
#define BLEControllerTypes_h

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace OSXBLE {
    typedef std::string BluetoothAddress;
    typedef std::string BluetoothUUID;
    typedef std::string DataChunk;

    typedef struct {
        BluetoothAddress address;
        std::string name;
    } DeviceDescriptor;

    typedef std::map<BluetoothUUID, std::vector<BluetoothUUID>> BluetoothServiceMap;

    class CallbackHolder {
    public:
        std::function<void()> callback_on_scan_start;
        std::function<void()> callback_on_scan_stop;
        std::function<void(DeviceDescriptor)> callback_on_scan_found;
        std::function<void()> callback_on_device_connected;
        std::function<void(std::string)> callback_on_device_disconnected;

        CallbackHolder():
        callback_on_scan_start([]() {}),
        callback_on_scan_stop([]() {}),
        callback_on_scan_found([](DeviceDescriptor) {}),
        callback_on_device_connected([]() {}),
        callback_on_device_disconnected([](std::string) {}) {}
    };
}
#endif
