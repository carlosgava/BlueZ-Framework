//
//  MacOS.h
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 29/07/25.
//


#include <chrono>
#include <thread>
#include <iostream>
#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>
#import <functional>

#define CENTRAL_MANAGER_TRIES 10
#define CENTRAL_MANAGER_SLEEP 250

@interface MacOS : NSObject<CBCentralManagerDelegate, CBPeripheralDelegate> {
    CBCentralManager *centralManager;
    dispatch_queue_t dispatchQueue;
}

@property (readonly, getter=isPeripheralReady) BOOL peripheralReady;
@property (readonly, getter=isCentralManagerPoweredOn) BOOL centralManagerPoweredOn;
@property (strong) CBPeripheral *peripheral;
@property std::function<void(CBPeripheral*, NSDictionary*, NSNumber*)> onScanFound;
@property std::function<void()> onConnected;
@property std::function<void(NSString*)> onDisconnected;
@property std::function<void(NSData*)> onCharacteristicUpdated;

-(void) setup;
-(void) startScan;
-(void) stopScan;
-(void) connect;
-(void) disconnect;
-(void) readValueOnCharacteristic:(CBUUID*)characteristic_uuid service:(CBUUID*)service_uuid;
-(void) subscribeToNotificationsOnCharacteristic:(CBUUID *)characteristic_uuid service:(CBUUID *)service_uuid;
-(void) subscribeToIndicationsOnCharacteristic:(CBUUID *)characteristic_uuid service:(CBUUID *)service_uuid;
-(void) writeData:(NSData*)data toCharacteristic:(CBUUID*)characteristic_uuid service:(CBUUID*)service_uuid;
-(void) writeNoResponseData:(NSData*)data toCharacteristic:(CBUUID*)characteristic_uuid service:(CBUUID*)service_uuid;
-(CBPeripheral *) getPeripheralWithUUID:(NSUUID *)uuid;

@end
