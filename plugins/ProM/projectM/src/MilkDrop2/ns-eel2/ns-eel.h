/*
  Nullsoft Expression Evaluator Library (NS-EEL)
  Copyright (C) 1999-2003 Nullsoft, Inc.
  
  ns-eel.h: main application interface header

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/


#ifndef __NS_EEL_H__
#define __NS_EEL_H__

// put standard includes here
#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
#define strcasecmp stricmp
#define strncasecmp _strnicmp
#endif

#ifndef EEL_F_SIZE
#define EEL_F_SIZE 8
#endif

#if EEL_F_SIZE == 4
typedef float EEL_F;
#else
typedef double EEL_F;
#endif


#ifdef __cplusplus
extern "C" {
#endif

// host should implement these (can be empty stub functions if no VM will execute code in multiple threads at once)

  // implement if you will be running the code in same VM from multiple threads, 
  // or VMs that have the same GRAM pointer from different threads, or multiple
  // VMs that have a NULL GRAM pointer from multiple threads.
  // if you give each VM it's own unique GRAM and only run each VM in one thread, then you can leave it blank.

  // or if you're daring....

void NSEEL_HOSTSTUB_EnterMutex();
void NSEEL_HOSTSTUB_LeaveMutex();


int NSEEL_init(); // returns 0 on success. clears any added functions as well

#define NSEEL_addfunction(name,nparms,code,len) NSEEL_addfunctionex((name),(nparms),(code),(len),0,0)
#define NSEEL_addfunctionex(name,nparms,code,len,pproc,fptr) NSEEL_addfunctionex2((name),(nparms),(code),(len),(pproc),(fptr),0)
void NSEEL_addfunctionex2(const char *name, int nparms, char *code_startaddr, int code_len, void *pproc, void *fptr, void *fptr2);

void NSEEL_quit();

int *NSEEL_getstats(); // returns a pointer to 5 ints... source bytes, static code bytes, call code bytes, data bytes, number of code handles
EEL_F *NSEEL_getglobalregs();

typedef void *NSEEL_VMCTX;
typedef void *NSEEL_CODEHANDLE;

NSEEL_VMCTX NSEEL_VM_alloc(); // return a handle
void NSEEL_VM_free(NSEEL_VMCTX ctx); // free when done with a VM and ALL of its code have been freed, as well

void NSEEL_VM_enumallvars(NSEEL_VMCTX ctx, int (*func)(const char *name, EEL_F *val, void *ctx), void *userctx); // return false from func to stop
void NSEEL_VM_resetvars(NSEEL_VMCTX ctx); // clears all vars to 0.0.

EEL_F *NSEEL_VM_regvar(NSEEL_VMCTX ctx, const char *name); // register a variable (before compilation)

void NSEEL_VM_freeRAM(NSEEL_VMCTX ctx); // clears and frees all (VM) RAM used
void NSEEL_VM_freeRAMIfCodeRequested(NSEEL_VMCTX); // call after code to free the script-requested memory
int NSEEL_VM_wantfreeRAM(NSEEL_VMCTX ctx); // want NSEEL_VM_freeRAMIfCodeRequested?

// if you set this, it uses a local GMEM context. 
// Must be set before compilation. 
// void *p=NULL; 
// NSEEL_VM_SetGRAM(ctx,&p);
// .. do stuff
// NSEEL_VM_FreeGRAM(&p);
void NSEEL_VM_SetGRAM(NSEEL_VMCTX ctx, void **gram); 
void NSEEL_VM_FreeGRAM(void **ufd); // frees a gmem context.
void NSEEL_VM_SetCustomFuncThis(NSEEL_VMCTX ctx, void *thisptr);


  // note that you shouldnt pass a C string directly, since it may need to 
  // fudge with the string during the compilation (it will always restore it to the 
  // original value though).
#ifdef __cplusplus
NSEEL_CODEHANDLE NSEEL_code_compile(NSEEL_VMCTX ctx, char *code, int lineoffs=0);
#else
NSEEL_CODEHANDLE NSEEL_code_compile(NSEEL_VMCTX ctx, char *code, int lineoffs);
#endif

char *NSEEL_code_getcodeerror(NSEEL_VMCTX ctx);
void NSEEL_code_execute(NSEEL_CODEHANDLE code);
void NSEEL_code_free(NSEEL_CODEHANDLE code);
int *NSEEL_code_getstats(NSEEL_CODEHANDLE code); // 4 ints...source bytes, static code bytes, call code bytes, data bytes
  

// global memory control/view
extern unsigned int NSEEL_RAM_limitmem; // if nonzero, memory limit for user data, in bytes
extern unsigned int NSEEL_RAM_memused;
extern int NSEEL_RAM_memused_errors;



// configuration:

#define NSEEL_MAX_VARIABLE_NAMELEN 16
// define this to override the max variable length (default is 16 bytes)

//#define NSEEL_MAX_TEMPSPACE_ENTRIES 2048
// define this to override the maximum working space in 8 byte units.
// 2048 is the default, and is way more than enough for most applications
// but in theory you might be able to come up with an expression big enough? maybe?


// maximum loop length
#define NSEEL_LOOPFUNC_SUPPORT_MAXLEN 1048576 // scary, we can do a million entries. probably will never want to, though.
#define NSEEL_LOOPFUNC_SUPPORT_MAXLEN_STR "1048576"



// when a VM ctx doesn't have a GRAM context set, make the global one this big
#define NSEEL_SHARED_GRAM_SIZE (1<<20)

// 128*65536 = ~8million entries. (64MB RAM used)
#define NSEEL_RAM_BLOCKS 128
#define NSEEL_RAM_ITEMSPERBLOCK 65536




#ifdef __cplusplus
}
#endif

#endif//__NS_EEL_H__
