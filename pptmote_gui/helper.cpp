#include "common.h"

void
ba2str( BTH_ADDR ba, char *addr )
{
    int i;
    unsigned char bytes[6];
    for( i=0; i<6; i++ ) {
        bytes[5-i] = (unsigned char) ((ba >> (i*8)) & 0xff);
    }
    sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5] );
}

int
str2uuid( const char *uuid_str, GUID *uuid)
{
    // Parse uuid128 standard format: 12345678-9012-3456-7890-123456789012
    int i;
    char buf[20] = { 0 };

    strncpy(buf, uuid_str, 8);
    uuid->Data1 = strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));

    strncpy(buf, uuid_str+9, 4);
    uuid->Data2 = (unsigned short) strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));
   
    strncpy(buf, uuid_str+14, 4);
    uuid->Data3 = (unsigned short) strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));

    strncpy(buf, uuid_str+19, 4);
    strncpy(buf+4, uuid_str+24, 12);
    for( i=0; i<8; i++ ) {
        char buf2[3] = { buf[2*i], buf[2*i+1], 0 };
        uuid->Data4[i] = (unsigned char)strtoul( buf2, NULL, 16 );
    }

    return 0;
}

int 
find_service(char *addrstr, char *uuidstr)
{
    // inquiry data structure
    DWORD qs_len = sizeof( WSAQUERYSET );
    WSAQUERYSETA *qs = (WSAQUERYSETA*) malloc( qs_len );

    DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_ALL;
    HANDLE h;
    int done = 0;
    int status = 0;

	GUID uuid = { 0 };
    char localAddressBuf[20] = { 0 };


    ZeroMemory( qs, qs_len );
    qs->dwSize = sizeof(WSAQUERYSET);
    qs->dwNameSpace = NS_BTH;
    qs->dwNumberOfCsAddrs = 0;
    qs->lpszContext = (LPSTR) localAddressBuf;

    
    strcpy( localAddressBuf, addrstr );

    if( strlen(uuidstr) != 36 || uuidstr[8] != '-' || uuidstr[13] != '-'
            || uuidstr[18] != '-' || uuidstr[23] != '-' ) {
        return 0;
    }

    str2uuid( uuidstr, &uuid );
    qs->lpServiceClassId = &uuid;

    status = WSALookupServiceBeginA( qs, flags, &h );

	if( SOCKET_ERROR == status) {
	
        int err_code = WSAGetLastError();
        if( WSASERVICE_NOT_FOUND == err_code ) {
            // this device does not advertise any services.  return an
            // empty list
            free( qs );
        } else {
            free(qs);
            return 0;
        }
    }

    // iterate through the inquiry results
	printf("* SDP listing on pptmote device...\n");
	printf("%-30s(%s)\n", "PPTMote", localAddressBuf);
	printf("%-30s%-15s%-10s%-10s\n", "** Service", "Description", "Protocol", "Port");
	printf("%-30s%-15s%-10s%-10s\n", "----------", "-----------", "--------", "------");
    while(!done) {

			status = WSALookupServiceNextA( h, flags, &qs_len, qs );
            if (NO_ERROR == status) {
            int proto;
            int port;

            CSADDR_INFO *csinfo = NULL;
       
            // set protocol and port
            csinfo = qs->lpcsaBuffer;
            if( csinfo != NULL ) {
			
                proto = csinfo->iProtocol;
                port = ((SOCKADDR_BTH*)csinfo->RemoteAddr.lpSockaddr)->port;

                if( proto == BTHPROTO_RFCOMM ) {
					printf("%-30s%-15s%-10s%-10i\n", qs->lpszServiceInstanceName, qs->lpszComment, "RFCOMM", port);
                } else if( proto == BTHPROTO_L2CAP ) {
                    printf("%-30s%-15s%-10s%-10i\n", qs->lpszServiceInstanceName, qs->lpszComment, "L2CAP", port);
                } else {
                    printf("%-30s%-15s%-10s%-10i\n", qs->lpszServiceInstanceName, qs->lpszComment, "UNKNOWN", port);
                }
            }
			} else {
                        int error = WSAGetLastError();
                       
                        if( error == WSAEFAULT ) {
                                free( qs );
                                qs = (WSAQUERYSETA*) malloc( qs_len );
                        } else if( error == WSA_E_NO_MORE ) {
                                done = 1;
                        } else {
								return 0;
                        }
            }


        }
		
        WSALookupServiceEnd( h );
        free( qs );

    return 0;
}

