//
//  BLEInternal.m
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 28/07/25.
//

#import <CoreBluetooth/CoreBluetooth.h>
#import <thread>

#import "BLEInternal.h"
#import "MacOS/MacOS.h"

using namespace OSXBLE;

MacOS *macable;
OSXBLEInternal::OSXBLEInternal() {}
OSXBLEInternal::~OSXBLEInternal() {}

void OSXBLEInternal::setup(CallbackHolder cb_holder) {
    this->callback_holder = cb_holder;

    macable = [[MacOS alloc]init];
    [macable setup];
    [macable setOnScanFound:[&](CBPeripheral* peripheral, NSDictionary* advData, NSNumber* rssi) {
        if (peripheral.name != nil) {
            DeviceDescriptor device;
            device.name = [peripheral.name cStringUsingEncoding:NSUTF8StringEncoding];
            device.address = [peripheral.identifier.UUIDString cStringUsingEncoding:NSUTF8StringEncoding];

            uint8_t prevSize = detected_addresses.size();
            detected_addresses.insert(device.address);

            if (prevSize != detected_addresses.size()) {
                callback_holder.callback_on_scan_found(device);
            }
        }
    }];
    [macable setOnConnected:[&]() {
        callback_holder.callback_on_device_connected();
    }];
    
    [macable setOnDisconnected:[&](NSString* msg) {
        std::string message = [msg cStringUsingEncoding:NSUTF8StringEncoding];
        callback_holder.callback_on_device_disconnected(message);
    }];
}

void OSXBLEInternal::scan_timeout(int32_t timeout_ms) {
    this->scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    this->scan_stop();
}

void OSXBLEInternal::scan_start() {
    [macable startScan];
    callback_holder.callback_on_scan_start();
}

void OSXBLEInternal::scan_stop() {
    [macable stopScan];
    callback_holder.callback_on_scan_stop();
}

bool OSXBLEInternal::scan_is_active() {
    //TODO: implement this
    return false;
}

bool OSXBLEInternal::is_connected() {
    return [macable isPeripheralReady] == YES ? true : false;
}


void OSXBLEInternal::connect(const BluetoothAddress &address) {
    //Ensure that the address is a valid UUID
    if (address.size() != 36) return;

    //Create a UUID object from std::string
    NSString *string = [[NSString alloc]initWithUTF8String:address.c_str()];
    NSUUID *uuid = [[NSUUID alloc]initWithUUIDString:string];

    CBPeripheral *peripheral = [macable getPeripheralWithUUID:uuid];
    [macable setPeripheral:peripheral];
    [macable connect];

    while (!this->is_connected());
}

void OSXBLEInternal::write_request(BluetoothUUID service, BluetoothUUID characteristic, DataChunk &_data) {
    NSString *serviceStr = [[NSString alloc]initWithUTF8String:service.c_str()];
    NSString *charStr = [[NSString alloc]initWithUTF8String:characteristic.c_str()];

    CBUUID *service_uuid = [CBUUID UUIDWithString:serviceStr];
    CBUUID *characteristic_uuid = [CBUUID UUIDWithString:charStr];

    NSData* data = [NSData dataWithBytes:_data.c_str() length:_data.size()];

    [macable writeData:data toCharacteristic:characteristic_uuid service:service_uuid];
}

void OSXBLEInternal::write_command(BluetoothUUID service, BluetoothUUID characteristic, DataChunk &_data) {
    NSString *serviceStr = [[NSString alloc]initWithUTF8String:service.c_str()];
    NSString *charStr = [[NSString alloc]initWithUTF8String:characteristic.c_str()];

    CBUUID *service_uuid = [CBUUID UUIDWithString:serviceStr];
    CBUUID *characteristic_uuid = [CBUUID UUIDWithString:charStr];

    NSData* data = [NSData dataWithBytes:_data.c_str() length:_data.size()];

    [macable writeNoResponseData:data toCharacteristic:characteristic_uuid service:service_uuid];
}

void OSXBLEInternal::read(BluetoothUUID service, BluetoothUUID characteristic,
                  std::function<void(const uint8_t *data, uint32_t length)> callback_on_read) {
    NSString *serviceStr = [[NSString alloc]initWithUTF8String:service.c_str()];
    NSString *charStr = [[NSString alloc]initWithUTF8String:characteristic.c_str()];

    CBUUID *service_uuid = [CBUUID UUIDWithString:serviceStr];
    CBUUID *characteristic_uuid = [CBUUID UUIDWithString:charStr];

    std::function<void(NSData* data)> onCharacteristicUpdated = macable.onCharacteristicUpdated;

    [macable setOnCharacteristicUpdated:[=](NSData* data) {
        [macable setOnCharacteristicUpdated:onCharacteristicUpdated];
        callback_on_read((const uint8_t*) data.bytes, (uint32_t) data.length);
    }];

    [macable readValueOnCharacteristic:characteristic_uuid service:service_uuid];
}

void OSXBLEInternal::notify(BluetoothUUID service, BluetoothUUID characteristic,
                    std::function<void(const uint8_t *data, uint32_t length)> callback_on_notify) {
    NSString *serviceStr = [[NSString alloc]initWithUTF8String:service.c_str()];
    NSString *charStr = [[NSString alloc]initWithUTF8String:characteristic.c_str()];

    CBUUID *service_uuid = [CBUUID UUIDWithString:serviceStr];
    CBUUID *characteristic_uuid = [CBUUID UUIDWithString:charStr];

    [macable setOnCharacteristicUpdated:[=](NSData* data) {
        callback_on_notify((const uint8_t*) data.bytes, (uint32_t) data.length);
    }];
    [macable subscribeToNotificationsOnCharacteristic:characteristic_uuid service:service_uuid];
}

void OSXBLEInternal::indicate(BluetoothUUID service, BluetoothUUID characteristic,
                std::function<void(const uint8_t *data, uint32_t length)> callback_on_indicate) {
    NSString *serviceStr = [[NSString alloc]initWithUTF8String:service.c_str()];
    NSString *charStr = [[NSString alloc]initWithUTF8String:characteristic.c_str()];

    CBUUID *service_uuid = [CBUUID UUIDWithString:serviceStr];
    CBUUID *characteristic_uuid = [CBUUID UUIDWithString:charStr];

    [macable setOnCharacteristicUpdated:[=](NSData* data) {
        callback_on_indicate((const uint8_t*) data.bytes, (uint32_t) data.length);
    }];
    [macable subscribeToNotificationsOnCharacteristic:characteristic_uuid service:service_uuid];
}

void OSXBLEInternal::unsubscribe(BluetoothUUID service, BluetoothUUID characteristic) {
    // TODO: IMPLEMENT
}

void OSXBLEInternal::disconnect() {
    [macable disconnect];
}
void OSXBLEInternal::dispose() {}
