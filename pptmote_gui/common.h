#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "resource.h"



#define PPT_DEVICE						  "pptdevice"
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CTX_WAIT						  0
#define CTX_RESP_LENGTH					  500
#define CTX_ICON						  9

ULONG NameToBthAddr(__in const LPWSTR pszRemoteName, __out PSOCKADDR_BTH pRemoteBtAddr);

void
ba2str( BTH_ADDR ba, char *addr );

int
str2uuid( const char *uuid_str, GUID *uuid);

int 
find_service(char *addrstr, char *uuidstr);
