#include <Shieldbot.h>
#include <IRSendRev.h>
#include <Streaming.h>

Shieldbot shieldbot = Shieldbot();

void setup()
{
    Serial.begin(115200);
    shieldbot.setMaxSpeed(255);//255 is max
    IR.Init(A5);
    cout << "hello world" << endl;
    
}

unsigned char dta[20];

void loop()
{
    if(IR.IsDta())
    {
        IR.Recv(dta);
        
        if(dta[0] != 9 || dta[6] != dta[7] || dta[8] != dta[9])
        {
            cout << "get data err" << endl;
        }
        else
        {

            //dta[6] = dta[6]*0.8;
            if(dta[6] == dta[8])
            {
                if(dta[6] <= 127)
                {
                    dta[6] = dta[6]*0.8;
                }
                else
                {
                    dta[6] = 255-dta[6];
                    dta[6] = 256-dta[6]*0.8;
                    
                }
            }
            
           
            cout << "get data" << endl;
            for(int i=0; i<4; i++)
            {
                cout << dta[6+i] << '\t';
            }
            cout << endl;

            shieldbot.drive(dta[6], dta[8]);

            if(dta[6] == 0 && dta[8] == 0)
            {
                shieldbot.fastStop();
            }

        }
        cout << endl;
    }
}


