/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __DEFINE_H
#define __DEFINE_H

#include <string>
#include <sys/types.h>

#include <ace/Basic_Types.h>
#include <ace/ACE_export.h>
#include <ace/Stack_Trace.h>
#include <ace/OS_NS_unistd.h>

#include "Log.hpp"

#define WPAssert( assertion ) { if (!(assertion)) { ACE_Stack_Trace st; sLog->outError( "\n%s:%i in %s ASSERTION FAILED:\n  %s\n%s\n", __FILE__, __LINE__, __FUNCTION__,  #assertion, st.c_str()); /*assert( #assertion &&0 )*/; ((void(*)())NULL)();} }
#define WPError( assertion, errmsg ) if ( ! (assertion) ) { sLog->outError( "%\n%s:%i in %s ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); /*assert( false );*/ }
#define WPWarning( assertion, errmsg ) if ( ! (assertion) ) { sLog->outError( "\n%s:%i in %s WARNING:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); }
#define WPFatal( assertion, errmsg ) if ( ! (assertion) ) { sLog->outError( "\n%s:%i in %s FATAL ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); ACE_OS::sleep(10); /*assert( #assertion &&0 );*/ abort(); }

#define ASSERT WPAssert

typedef ACE_INT64 int64;
typedef ACE_INT32 int32;
typedef ACE_INT16 int16;
typedef ACE_INT8 int8;
typedef ACE_UINT64 uint64;
typedef ACE_UINT32 uint32;
typedef ACE_UINT16 uint16;
typedef ACE_UINT8 uint8; 

#define _GUARD(MUTEX, LOCK) \
  ACE_Guard< MUTEX > _GUARD_OBJECT (LOCK); \
    if (_GUARD_OBJECT.locked() == 0) ASSERT(false);

//! For proper implementation of multiple-read, single-write pattern, use
//! ACE_RW_Mutex as underlying @MUTEX
# define _WRITE_GUARD(MUTEX, LOCK) \
  ACE_Write_Guard< MUTEX > _GUARD_OBJECT (LOCK); \
    if (_GUARD_OBJECT.locked() == 0) ASSERT(false);

//! For proper implementation of multiple-read, single-write pattern, use
//! ACE_RW_Mutex as underlying @MUTEX
# define _READ_GUARD(MUTEX, LOCK) \
  ACE_Read_Guard< MUTEX > _GUARD_OBJECT (LOCK); \
    if (_GUARD_OBJECT.locked() == 0) ASSERT(false);

#endif
