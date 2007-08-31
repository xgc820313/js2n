/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * Copyright (c) 2007 Optimistec Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/* 
* Revision 1.1  2007/08/31 16:14:58  amnond
* Initial revision
*/
////////////////////////////////////////////////////////////
// nprn.cpp : js2n framework /  command dispatcher for plugins
//

#ifdef XP_UNIX
#include "binreloc.h"
#endif

#include "nprn.h"
#include <ptlib.h>
#include <ptlib/pprocess.h>


class PInvokeMethod;

PDictionary<PString,PDynaLink>		g_File2DynaLink;
PDictionary<PString,PInvokeMethod>	g_Class2Invoke;
PDictionary<PString,PInvokeMethod>	g_ObjID2Invoke;


class PWLibProcess : public PProcess
{
    PCLASSINFO(PWLibProcess, PProcess)
  public:
    PWLibProcess();

    void Main();
};



PWLibProcess::PWLibProcess()
  : PProcess("pwlibproc", "jsex",
             1, 1 )
{
	// The objects will already be deleted by g_Class2Invoke destructor
	g_ObjID2Invoke.DisallowDeleteObjects();
}

void PWLibProcess::Main()
{
  // Empty function
}



PWLibProcess g_pwlibProcess;

#ifdef _WINDOWS
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif
////////////////////////////////////////////////////////////

NPBool tNativeLogic::Init( NPP npp, NPWindow* pNPWindow )
{
    m_NPP		= npp;
    m_pNPWindow     = pNPWindow;
    m_nObjId.SetValue( 0 );
        
#ifdef XP_UNIX
    BrInitError error;
    if (br_init_lib(&error) == 0 && error != BR_INIT_ERROR_DISABLED)
    {
        printf( "Error - can't init binreloc, code %d\n", error );
    }
#endif
    return TRUE;
}

void tNativeLogic::PostEventToPluginThread( const PString& strEvent )
{
	// This should be called from the thread that handles the event
	// It will eventually a trigger to SendEventToJS
}

void tNativeLogic::SendEventToJS( const PString& strEvent )
{
	// This should only be called within the context of the plugin thread
	PString strJSCall = "javascript:callback('" + strEvent + "')"; 
	NPN_GetURL( m_NPP, strJSCall,"_self");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern tNativeLogic g_NativeLogic;
void SendPluginEvent( const char* szEvent )
{
	PString strEvent = szEvent;
	g_NativeLogic.SendEventToJS( strEvent );
}
//////////////////////////////////////////////////////////////////////
typedef void (*SendPluginEv)( const char* szEvent );
typedef char* (*SetEventFunc)(SendPluginEv funcPtr);
typedef char* (*InvokeFunc)(const char* szCommand);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PInvokeMethod : public PObject
{
private:
	PInvokeMethod( void )
	{
	}

public:
	PInvokeMethod( InvokeFunc pInvokefunc )
	{
		m_pInvokeFunc	= pInvokefunc;
	}

	InvokeFunc	m_pInvokeFunc;
};

PString tNativeLogic::InvokeFunction( const PString& strFunction )
{
	PString strResult = "Ok";
	PStringArray arParams = strFunction.Tokenise( " " );
	PString strCommand = arParams[ 0 ];
	if ( strCommand == "Require" )
	{
		// Requested use of a JS extension plugin
		PString strLibrary = arParams[ 1 ];

		if ( g_File2DynaLink.GetAt( strLibrary ) != NULL )
		{
			// This JS extension plugin has already been loaded
			return strResult;
		}

		PDynaLink* pDynaLink = new PDynaLink();
		PString strExt = pDynaLink->GetExtension();

		PString strPath;
#ifdef XP_UNIX
                strPath = br_find_exe_dir("/usr/lib/firefox/plugins/");
                strPath += "js2n/";
#else
		//Get Current Path of exe
		char szFullFileName[ MAX_PATH ];
		GetModuleFileName ( GetModuleHandle(NULL), szFullFileName, MAX_PATH ) ;
		strPath = szFullFileName;
		PINDEX nPos = strPath.FindLast( "\\" );
		strPath = strPath.Mid( 0, nPos + 1 ) + "plugins\\js2n\\";
#endif
		PString strDynaLib = strPath + strLibrary + strExt;
		if ( !pDynaLink->Open( strDynaLib ) )
		{
			delete pDynaLink;
			return "Error Can't find " + strDynaLib;
		}

		// Get the function to set the callback and the receive the list
		// of objects this JS extension supports
		SetEventFunc setEventFunc;
		if ( !pDynaLink->GetFunction( "SetEventFunc", (PDynaLink::Function&)setEventFunc ) )
		{
                    PString strError = "";
#ifdef XP_UNIX
                    char* szError;
                    if ((szError = dlerror()) != NULL)
                    {
                        strError = szError;
                    }

                    if ((szError = dlerror()) != NULL)
                    {
                        strError = szError;
                    }
#else
                    int nErrorCode = GetLastError();
                    LPTSTR s;
                    if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                    FORMAT_MESSAGE_FROM_SYSTEM,
                                    NULL,
                                    nErrorCode,
                                    0,
                                    (LPTSTR)&s,
                                    0,
                                    NULL))
                    {
                            strError = s;
                    }
                    else
                    {
                            strError = psprintf( "Error code %d", nErrorCode ); 
                    }
#endif
                    delete pDynaLink;
                    return "Error Can't find SetEventFunc." + strError;
		}

		// Set the callback function this JS extension plugin can invoke
		// whenever an event is generated
		PString strObjList = setEventFunc( SendPluginEvent );

		// Get the pointer of the command invocation function for this
		// JS extension plugin
		InvokeFunc invokeFunc;
		if ( !pDynaLink->GetFunction( "InvokeFunction", (PDynaLink::Function&)invokeFunc ) )
		{
			delete pDynaLink;
			return "Error Can't find InvokeFunction";
		}

		// Store the pDynaLink structure in the hash table, when
		// the hash table will be cleared, all the pointers of
		// PDynaLink will be deleted, unloading the shared libraries
		// that they represent.
		g_File2DynaLink.SetAt( strLibrary, pDynaLink );

		// Create an object to store this pointer. This pointer
		// will be deleted when g_Class2Invoke is cleared or destroyed

		PInvokeMethod* pInvokeMethod = new PInvokeMethod( invokeFunc );

		PStringArray arObjects = strObjList.Tokenise( "," );
		int nTotal = arObjects.GetSize();
		for (int i=0; i<nTotal; i++)
		{
			PString strClassName = arObjects[ i ];
			g_Class2Invoke.SetAt( strClassName, pInvokeMethod );
		}
	}
	else
	if ( strCommand == "CreateObject" )
	{
		PString strClassName = arParams[ 1 ];
		PInvokeMethod* pInvokeMethod = g_Class2Invoke.GetAt( strClassName );
		if ( pInvokeMethod == NULL )
		{
			return "Error Can't find " + strClassName;
		}
		PString strId = GetObjectId();
		PString strExtCommand = "CreateObj " + strClassName + " " + strId;
		PString strVal = pInvokeMethod->m_pInvokeFunc( (const char*)strExtCommand );
		if ( strVal.Left( 2 ) != "Ok" )
		{
			return "Error Can't find method " + strExtCommand;
		}

		g_ObjID2Invoke.SetAt( strId, pInvokeMethod ); 
		PString strRet = "Ok " + strId;
		return strRet;
	}
	else
	if ( strCommand == "InvokeMethod" )
	{
		PString strObjId = arParams[ 1 ];
		PInvokeMethod* pInvokeMethod = g_ObjID2Invoke.GetAt( strObjId ); 
		if ( pInvokeMethod == NULL )
		{
			return "Error Can't find object for id " + strObjId;
		}
		PString strVal = pInvokeMethod->m_pInvokeFunc( (const char*)strFunction );
		return strVal;
	}
	return strResult;
}

PString tNativeLogic::GetObjectId( void )
{
	PString strObjId;
	strObjId.sprintf( "%i", ++m_nObjId );
	return strObjId;
}
