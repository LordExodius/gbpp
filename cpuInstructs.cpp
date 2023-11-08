#include "cpuInstructs.h"
CPUInstructions::CPUInstructions() {

}

int CPUInstructions::ExecuteNextOpcode( )
{
   int res = 0
   BYTE opcode = ReadMemory(m_ProgramCounter) ;
   m_ProgramCounter++ ;
   res = ExecuteOpcode(opcode) ;
   return res ;
} 

int CPUInstructions::ExecuteOpcode(BYTE opcode)
{
   switch(opcode)
   {
   case 0x06:
   CPU_8BIT_LOAD(m_RegisterBC.hi) ;
   return 8;

   case 0x80:
   CPU_8BIT_ADD(m_RegisterAF.hi, m_RegisterBC.hi,false,false) ;
   return 4;

   case 0x90:
   CPU_8BIT_SUB(m_RegisterAF.hi, m_RegisterBC.hi,false,false) ;
   return 4 ;

   case 0xAF:
   CPU_8BIT_XOR(m_RegisterAF.hi, m_RegisterAF.hi, false) ;
   return 4;

   case 0x20 :
   CPU_JUMP_IMMEDIATE( true, FLAG_Z, false ) ;
   return 8;

   case 0xCC :
   CPU_CALL( true, FLAG_Z, true) ;
   case 0xD0:
   CPU_RETURN( true, FLAG_C, false ) ;
   return 8;

   case 0xCB:
   return ExecuteExtendedOpcode( ) ;

   default:
   assert(false); return 0 ;// unhandled opcode } 