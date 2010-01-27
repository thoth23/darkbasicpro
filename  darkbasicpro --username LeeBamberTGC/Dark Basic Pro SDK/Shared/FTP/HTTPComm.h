//
// HTTP Comm Header
//

// Functions to access an HTTP Secure Server
bool	HTTP_Connect		( char* url );
bool	HTTP_Connect		( char* url, WORD port );
LPSTR	HTTP_RequestData	( LPSTR pVerb, LPSTR pObjectName, LPSTR pHeaderString, DWORD dwHeaderSize, LPSTR pPostData, DWORD dwPostDataSize );
LPSTR	HTTP_RequestData	( LPSTR pVerb, LPSTR pObjectName, LPSTR pHeaderString, DWORD dwHeaderSize, LPSTR pPostData, DWORD dwPostDataSize, DWORD dwAccessFlag );
bool	HTTP_Disconnect		( void );
