

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Sep 02 00:04:12 2007
 */
/* Compiler settings for .\Js2n.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_Js2nLib,0xD1D52419,0x786B,0x4DF4,0xBF,0xBA,0xDB,0x53,0x43,0x95,0x42,0xE1);


MIDL_DEFINE_GUID(IID, DIID__DJs2n,0xC92682EB,0xF002,0x4FA9,0x94,0x30,0xE5,0xB1,0x81,0xB9,0x06,0x20);


MIDL_DEFINE_GUID(IID, DIID__DJs2nEvents,0x7EE9678C,0xE65B,0x424C,0x88,0x31,0x83,0x55,0x80,0xDF,0x30,0xFF);


MIDL_DEFINE_GUID(CLSID, CLSID_Js2n,0xA5243CDA,0x45BB,0x408E,0x8B,0xB6,0xF9,0x99,0x69,0x2C,0x77,0xA3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif


