#include "Arduino.h"


//TODO: Those defines should be definable from the main sketch -------------------------------------------------------------------------------------------
#define CATH_MAX_TASKS    9         // Max Number of task instances. MUST BE >= to tasks instancied
#define CATH_SMALL_COUNTER          // set the max period to 65536 ms, else (when commented) 32 bit delay will be allowed


//- Definitions ------------------------------------------------------------------------------------------

#ifdef CATH_SMALL_COUNTER
typedef uint16_t CathCnt;
#else
typedef uint32_t CathCnt;
#endif


class Cath{

  public:

// Derived class MUST implement these 2 methods
  virtual void          SetUp() =0;                 // Called at setup
  virtual void          Loop()  =0;                 // Called periodically

  CathCnt               m_CurCounter;               // Curent number of ms before next Loop call
  CathCnt               m_LoopDelay;                // Default period of Loop call (in ms)

  static uint8_t        S_NbTask;                   // Actual number of task instances
  static Cath*          S_CathTasks[CATH_MAX_TASKS];  // Array of task object pointers
  static uint8_t        S_LastMilli;                // Used to call every ms (a byte is enought to detect change)


  static void S_Register(Cath* Task,CathCnt Period,CathCnt Offset=1);
  static void S_SetUp();
  static void S_Loop();

};
