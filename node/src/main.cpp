//
// Created by Benjamin Skirlo on 18/01/17.
//

#include "Arduino.h"
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#define Serial (Serial ? Serial : Serial1)

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the prototype TTN
// network initially.
static const u1_t PROGMEM APPEUI[8]={ 0xB2, 0x30, 0x00, 0xF0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// The key shown here is the semtech default key.
static const u1_t PROGMEM APPKEY[16] = { 0x8D, 0x32, 0xB9, 0x8E, 0x2F, 0xAE, 0x08, 0x77, 0xB1, 0x8E, 0xCB, 0x9F, 0x3D, 0xDA, 0x6B, 0xF5 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
//struct Data{
//    int measurement = 255;
//} * myData;
uint16_t myData[4] = {0,0,0,0};
//static uint8_t mydata[] = "Hello, world!";
static osjob_t sendjob;
static osjob_t measurementjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL      = 30;
const unsigned MEASURE_INTERVAL = 15;

void do_send(osjob_t* j);

// Pin mapping
const lmic_pinmap lmic_pins = {
        .nss = 10,
        .rxtx = LMIC_UNUSED_PIN,
        .rst = 9,
        .dio = {2, 6, 7},
};

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
                u4_t netid = 0;
                devaddr_t devaddr = 0;
                u1_t nwkKey[16];
                u1_t artKey[16];
//                LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
                Serial.print("netid: ");
                Serial.println(netid, DEC);
                Serial.print("devaddr: ");
                Serial.println(devaddr, HEX);
                Serial.print("artKey: ");
                for (int i=0; i<sizeof(artKey); ++i) {
                    Serial.print(artKey[i], HEX);
                }
                Serial.println("");
                Serial.print("nwkKey: ");
                for (int i=0; i<sizeof(nwkKey); ++i) {
                    Serial.print(nwkKey[i], HEX);
                }
                Serial.println("");
                digitalWrite(13, LOW);

//                LMIC_setSeqnoUp(140);
            }
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
                Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
                Serial.println(F("Received "));
                Serial.println(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, (uint8_t *)myData, sizeof(myData), 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}


void do_measure(osjob_t* j){

    Serial.println("make new measurements");
    uint16_t p0 = analogRead(0);
    uint16_t p1 = analogRead(1);
    uint16_t p2 = analogRead(2);
    uint16_t p3 = analogRead(3);

    int sum = p0+p1+p2+p3;
    Serial.println  ("Accumulated measurements");
    Serial.println(p0);
    Serial.println(p1);
    Serial.println(p2);
    Serial.println(p3);
    Serial.println(sum);

    myData[0] = p0;
    myData[1] = p1;
    myData[2] = p2;
    myData[3] = p3;

    os_setTimedCallback(j, os_getTime()+sec2osticks(MEASURE_INTERVAL), do_measure);
    // Check if there is not a current TX/RX job running
}


void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting"));

#ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
#endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    do_measure(&measurementjob);
    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}

void loop() {

    os_runloop_once();
}
