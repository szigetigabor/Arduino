#pragma once
#ifndef TEMP_CONFIG_H
#define TEMP_CONFIG_H

#define NR_OF_SENSORS 3
#define ONEWIRE_PIN D5

// define index for a sensors
#define INTAKE 0
#define POOL   1
#define IN     2
#define OUT    3

static byte addr1[8]={0x28, 0x35, 0x49, 0x79, 0x97, 0x10, 0x03, 0x65};
static byte addr2[8]={0x28, 0xD8, 0x68, 0x79, 0x97, 0x10, 0x03, 0x33};
static byte addr3[8]={0x28, 0x10, 0xb2, 0x79, 0x97, 0x10, 0x03, 0xee};

static byte* sensorAddresses[NR_OF_SENSORS] = {addr1, addr2, addr3};
#endif
