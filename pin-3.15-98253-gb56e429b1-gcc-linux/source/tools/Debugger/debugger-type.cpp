/*
 * Copyright 2002-2019 Intel Corporation.
 * 
 * This software is provided to you as Sample Source Code as defined in the accompanying
 * End User License Agreement for the Intel(R) Software Development Products ("Agreement")
 * section 1.L.
 * 
 * This software and the related documents are provided as is, with no express or implied
 * warranties, other than those that are expressly stated in the License.
 */

/*
 * Simple test of the PIN_GetDebuggerType() API.
 */

#include <cstdlib>
#include <iostream>
#include "pin.H"

BOOL IsFirstIns = TRUE;

static void InstrumentIns(INS, VOID *);


int main(int argc, char * argv[])
{
    PIN_InitSymbols();
    PIN_Init(argc, argv);

    INS_AddInstrumentFunction(InstrumentIns, 0);
    PIN_StartProgram();
    return 0;
}

static void InstrumentIns(INS ins, VOID *)
{
    if (IsFirstIns)
    {
#if defined(TARGET_MAC)
        if (PIN_GetDebugStatus() != DEBUG_STATUS_CONNECTED)
        {
            // On macOS in launch mode, a debugger connection is established after the application has already started running
            // in the earliest point it is safe to do so. So, Jitting of the application start before we reach that point.
            // Therefore we return here until the the debugger connection session has started.
            return;
        }
#endif
        IsFirstIns = FALSE;

        // The debugger isn't connected until after PIN_StartProgram().  Therfore, we
        // call the API when instrumenting the first instruction.
        //
        if (PIN_GetDebuggerType() == DEBUGGER_TYPE_GDB)
            std::cout << "Debugger Type is GDB" << std::endl;
        else if (PIN_GetDebuggerType() == DEBUGGER_TYPE_LLDB)
            std::cout << "Debugger Type is LLDB" << std::endl;
        else
            std::cout << "Got wrong Debugger Type from Pin" << std::endl;
    }
}
