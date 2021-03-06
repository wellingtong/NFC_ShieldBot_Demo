#include <IRSendRev.h>
#include <Streaming.h>
#include <PN532.h>
#include <SPI.h>

/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define PN532_CS 9
PN532 nfc(PN532_CS);

unsigned char dtaUart[20];
unsigned char dtaLen = 0;
unsigned char getDta = 0;

#define  NFC_DEMO_DEBUG 1

void nfc_init()
{
    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
#ifdef NFC_DEMO_DEBUG
        Serial.print("Didn't find PN53x board");
#endif
        while (1); // halt
    }
#ifdef NFC_DEMO_DEBUG
    // Got ok data, print it out!
    Serial.print("Found chip PN5");
    Serial.println((versiondata>>24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata>>16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata>>8) & 0xFF, DEC);
    Serial.print("Supports ");
    Serial.println(versiondata & 0xFF, HEX);
#endif
    // configure board to read RFID tags and cards
    nfc.SAMConfig();
}
void setup()
{

    Serial.begin(115200);

    nfc_init();
    cout << "hello world" << endl;
}

void loop()
{

    if(getDta)
    {
        cout << "get Data:" << dtaUart[0] << endl;
        getDta = 0;
        dtaLen = 0;


        uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

        if (id != 0)
        {
#ifdef NFC_DEMO_DEBUG
            Serial.print("Read card #");
            Serial.println(id);
            Serial.println();
#endif
            uint8_t keys[]= {
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF    };
            uint8_t writeBuffer[16];
            for(uint8_t i = 0;i < 16;i ++)
            {
                writeBuffer[i]=dtaUart[0]; //Fill buffer with 0,1,2....F
            }
            if(nfc.authenticateBlock(1, id ,0x08,KEY_A,keys)) //authenticate block 0x08
            {
                //if authentication successful
                unsigned char written = 0;
                if(written == 0) //Not written
                {
                    written = nfc.writeMemoryBlock(1,0x08,writeBuffer); // Write writeBuffer[] to block 0x08
                    if(written)
#ifdef NFC_DEMO_DEBUG
                    Serial.println("Write Successful");
#endif
                }
            }
            else
            {
                cout << "authentic err" << endl;
            }
        }
    }
}


void serialEvent()
{
    while(Serial.available())
    {
        dtaUart[dtaLen++] = Serial.read();

        if(dtaUart[dtaLen-1] == '\n')
        {
            getDta = 1;
        }
    }
}
