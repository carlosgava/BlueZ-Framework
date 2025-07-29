//
//  BLEController.cpp
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 28/07/25.
//

#include <iostream>
#include "BLEController.h"
#include "../ObjC/BLEInternal.h"

using namespace OSXBLE;

OSXBLEController::OSXBLEController() : internal(nullptr) { internal = new OSXBLEInternal(); }

OSXBLEController::~OSXBLEController() {
    if (internal != nullptr) {
        delete internal;
    }
}

void OSXBLEController::setup(CallbackHolder callback_holder) { internal->setup(callback_holder); }

void OSXBLEController::scan_start() { internal->scan_start(); }

void OSXBLEController::scan_stop() { internal->scan_stop(); }

bool OSXBLEController::scan_is_active() { return internal->scan_is_active(); }

void OSXBLEController::scan_timeout(int32_t timeout_ms) { internal->scan_timeout(timeout_ms); }

bool OSXBLEController::is_connected() { return internal->is_connected(); }

void OSXBLEController::connect(const BluetoothAddress& address) { internal->connect(address); }

void OSXBLEController::write_request(BluetoothUUID service, BluetoothUUID characteristic, DataChunk data) {
    internal->write_request(service, characteristic, data);
}

void OSXBLEController::write_command(BluetoothUUID service, BluetoothUUID characteristic, DataChunk data) {
    internal->write_command(service, characteristic, data);
}

void OSXBLEController::read(BluetoothUUID service, BluetoothUUID characteristic,
                               std::function<void(const uint8_t* data, uint32_t length)> callback_on_read) {
    internal->read(service, characteristic, callback_on_read);
}

void OSXBLEController::notify(BluetoothUUID service, BluetoothUUID characteristic,
                                 std::function<void(const uint8_t* data, uint32_t length)> callback_on_notify) {
    internal->notify(service, characteristic, callback_on_notify);
}

void OSXBLEController::indicate(BluetoothUUID service, BluetoothUUID characteristic,
                                   std::function<void(const uint8_t* data, uint32_t length)> callback_on_indicate) {
    internal->indicate(service, characteristic, callback_on_indicate);
}

void OSXBLEController::unsubscribe(BluetoothUUID service, BluetoothUUID characteristic) {
    internal->unsubscribe(service, characteristic);
}

void OSXBLEController::disconnect() { internal->disconnect(); }

void OSXBLEController::dispose() { internal->dispose(); }
