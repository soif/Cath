#include "Cath.h"


//****************************************************************************************************************
// I/O Abstraction

#define kOutPinSlowBlink  4     // Output pins where different leds are connected
#define kOutPinFastBlink  5     
#define kOutPinAssyBlink  6
#define kOutPinAorB       7
#define kOutPinAandB      8
#define kOutPinAxorB      9

#define kInPinA           2     // Input pins where pushbuttons are connected (Internal pullup required)
#define kInPinB           3


//****************************************************************************************************************
// Globals

bool gPushA=false;      // Memory state of button A, true if pushed, debounced
bool gPushB=false;      // Memory state of button B, true if pushed, debounced

//Exemple task ...........................................................................................

//Blinker ..........................................................................................
class Blinker: public Cath{

  public:

  Blinker(uint8_t Pin,unsigned long Period,unsigned long Offset=1){
    m_Pin=Pin;
    Cath::S_Register(this,Period,Offset);
  }

  void SetUp(){
    pinMode(m_Pin,OUTPUT);
    digitalWrite(m_Pin, LOW);
  }

  void Loop(){
    digitalWrite(m_Pin,!digitalRead(m_Pin));
  }

  uint8_t     m_Pin;    // Number id of the output pin to blink
};

//Assy Blinker ..........................................................................................
class ABlinker: public Cath{

  public:

  ABlinker(){
    Cath::S_Register(this,2000,666);
  }

  void SetUp(){
    pinMode(kOutPinAssyBlink,OUTPUT);
    digitalWrite(kOutPinAssyBlink, LOW);
    m_State=0;
  }

  void Loop(){

    if (m_State++ & 1){
       digitalWrite(kOutPinAssyBlink,HIGH);
       m_CurCounter=50;                       // Here we force the next call to a shorter value
    }
    else {
      digitalWrite(kOutPinAssyBlink,LOW);
    }
  }

  unsigned char m_State;    // State counter, short pulse if Odd
};

//PushBut ..........................................................................................
class PushBut: public Cath{

  public:

  PushBut(uint8_t Pin, bool* Store){
    m_Pin=Pin;
    m_Store=Store;
    Cath::S_Register(this,20);    // Here 20ms is for switch deboudcing
  }

  void SetUp(){
    pinMode(m_Pin,INPUT_PULLUP);
  }

  void Loop(){
    *m_Store=!digitalRead(m_Pin); // We invert the value because the switch in inverted (connected to ground)
  }

    uint8_t     m_Pin;      // Number Id of input Pin to test
    bool*       m_Store;    // Boolean value to store the result
};

//PinAorB ..........................................................................................
class PinAorB: public Cath{

  public:

  PinAorB(){
    Cath::S_Register(this,20);
  }

  void SetUp(){
    pinMode(kOutPinAorB,OUTPUT);
  }

  void Loop(){
    digitalWrite(kOutPinAorB, gPushA || gPushB);
  }
};

//PinAandB ..........................................................................................
class PinAandB: public Cath{

  public:

  PinAandB(){
    Cath::S_Register(this,20);
  }

  void SetUp(){
    pinMode(kOutPinAandB,OUTPUT);
  }

  void Loop(){
    digitalWrite(kOutPinAandB, gPushA && gPushB);
  }
};

//PinAxorB ..........................................................................................
class PinAxorB: public Cath{

  public:

  PinAxorB(){
    Cath::S_Register(this,60);
  }

  void SetUp(){
    pinMode(kOutPinAxorB,OUTPUT);
  }

  void Loop(){
    digitalWrite(kOutPinAxorB, !gPushA != !gPushB && !digitalRead(kOutPinAxorB));
  }
};

//****************************************************************************************************************
// Global tasks instanciation

// 3 Instances of the blinker task
Blinker   BuiltIn(LED_BUILTIN,100,50);
Blinker   Blinker1(kOutPinSlowBlink,500);
Blinker   Blinker2(kOutPinFastBlink,1500,500);

// 2 Instances of the PushBut task
PushBut   PushA(kInPinA,&gPushA);
PushBut   PushB(kInPinB,&gPushB);

// 1 instance by tasks for these one
ABlinker  Assy;
PinAorB   AorB;
PinAandB  AandB;
PinAxorB  AxorB;


//-----------------------------------------------------------------------------------------------------------------
void setup() {
  Cath::S_SetUp();    // Just ask Cath to call the task's setup
}

//-----------------------------------------------------------------------------------------------------------------
void loop() {
  Cath::S_Loop();    // Just ask Cath to call the task's loop
}

// That's all folks.....
