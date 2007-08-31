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

#ifdef _WINDOWS
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NPRN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NPRN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef NPRN_EXPORTS
#define NPRN_API __declspec(dllexport)
#else
#define NPRN_API __declspec(dllimport)
#endif
#endif

#include <npapi.h>
#include <ptlib.h>

#ifdef _WINDOWS
// This class is exported from the compiled nprn.dll
class NPRN_API CNprn {
public:
	CNprn(void);
	// TODO: add your methods here.
};

extern NPRN_API int nNprn;

NPRN_API int fnNprn(void);
#endif

class tNativeLogic
{
public:
	NPBool Init( NPP npp, NPWindow* pNPWindow );
	void PostEventToPluginThread( const PString& strEvent );
	void SendEventToJS( const PString& strEvent );
	PString InvokeFunction( const PString& strFunction );

private:
	PString GetObjectId( void );

private:
	NPP				m_NPP;
	NPWindow*		m_pNPWindow;
	PAtomicInteger	m_nObjId;
};
