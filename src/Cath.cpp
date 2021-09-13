/*
Cath Copyright Cyrob 2021
Cyrob Arduino Task Helper by Philippe Demerliac

See my presentation video in French : https://youtu.be/aGwHYCcQ3Io
See also for v1.3 : https://youtu.be/ph57EpJPs5E

=====================================================================================
==========================   OPEN SOURCE LICENCE   ==================================
=====================================================================================

Copyright 2021 Philippe Demerliac Cyrob.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

................................................................................................................
Release history
................................................................................................................
Version Date        Author    Comment
1.0     30/08/2021  Phildem   First version tested ok
1.1     05/09/2021  Phildem   Misc fixes, better comments and presentation
1.2     06/09/2021  Phildem   Remove unnecessary Cath:: in Cath class definition, (Warning removed)
1.3     08/09/2021  Phildem   Misc comments/name fixes, Memory optimised, __CathOpt_SmallCounter__ option added
1.4     13/09/2021  Soif      Fixes english comments & indentation
1.5     13/09/2021  Soif      Move to a standart Arduino Library

*/

//____________________________________________________________________________________________________________

#include "Cath.h"

//Cath static variables definitions 
//(Note set to 0 for code clarity but done by default anyway because they are static)
uint8_t       Cath::S_NbTask=0;
Cath*         Cath::S_CathTasks[CATH_MAX_TASKS];
uint8_t       Cath::S_LastMilli=0;


//..............................................................
// Must be called in task constructors to register in the task list
// WARNING : think to set CATH_MAX_TASKS as needed
// Task   : Pointer to the derivated task to register
// Period : Loop call Period (in ms). WARNING do not pass 0!
// Offset : Delay of the first call in ms (1 def). WARNING do not pass 0!
void Cath::S_Register(Cath* Task,CathCnt Period,CathCnt Offset){
  Task->m_LoopDelay=Period;
  Task->m_CurCounter= Offset;
  Cath::S_CathTasks[Cath::S_NbTask++]=Task;
}

//..............................................................
// Must be called once in Arduino setup to call all the task setups
void Cath::S_SetUp(){
  for(int T=0;T<S_NbTask;T++)
    Cath::S_CathTasks[T]->SetUp();
}

  //..............................................................
// Must be called once in Arduino Loop to call all the task loop if needed
void Cath::S_Loop(){
  uint8_t CurMilli=millis();
  if (CurMilli!=S_LastMilli) {
    S_LastMilli=CurMilli;
    for(int T=0;T<S_NbTask;T++) 
      if ( Cath::S_CathTasks[T]->m_CurCounter--==0) {
        Cath::S_CathTasks[T]->m_CurCounter=Cath::S_CathTasks[T]->m_LoopDelay;
        Cath::S_CathTasks[T]->Loop();
      }
    }
}
