/*
  Nullsoft Expression Evaluator Library (NS-EEL)
  Copyright (C) 1999-2003 Nullsoft, Inc.
  
  ns-eel-addfuncs.h: defines macros useful for adding functions to the compiler

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

#ifndef __NS_EEL_ADDFUNCS_H__
#define __NS_EEL_ADDFUNCS_H__

#ifdef __cplusplus
extern "C" {
#endif

struct _compileContext;
typedef void (*NSEEL_PPPROC)(void *data, int data_size, struct _compileContext *userfunc_data);

void NSEEL_PProc_RAM(void *data, int data_size, struct _compileContext *ctx);
void NSEEL_PProc_THIS(void *data, int data_size, struct _compileContext *ctx);


void _asm_generic3parm(void); // 3 double * parms, returning double *
void _asm_generic3parm_end(void);
void _asm_generic3parm_retd(void); // 3 double * parms, returning double
void _asm_generic3parm_retd_end(void);
void _asm_generic2parm(void); // 2 double * parms, returning double *
void _asm_generic2parm_end(void);
void _asm_generic2parm_retd(void); // 2 double * parms, returning double
void _asm_generic2parm_retd_end(void);
void _asm_generic1parm(void); // 1 double * parms, returning double *
void _asm_generic1parm_end(void);
void _asm_generic1parm_retd(void); // 1 double * parms, returning double 
void _asm_generic1parm_retd_end(void);

void _asm_megabuf(void);
void _asm_megabuf_end(void);



#if EEL_F_SIZE == 4
#define EEL_F_SSTR "4"
#define EEL_F_SUFFIX "s"
#else
#define EEL_F_SSTR "8"
#define EEL_F_SUFFIX "l"
#endif

#ifdef _MSC_VER
#define NSEEL_CGEN_CALL __cdecl
#else
#define NSEEL_CGEN_CALL 
#endif

#ifdef __cplusplus
};

#endif
#endif//__NS_EEL_ADDFUNCS_H__
