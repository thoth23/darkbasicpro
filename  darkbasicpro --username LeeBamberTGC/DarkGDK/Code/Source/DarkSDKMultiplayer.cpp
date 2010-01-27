
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDKMultiplayer.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CreateNetGame						( LPSTR gamename, LPSTR name, int playermax );
void	CreateNetGameEx						( LPSTR gamename, LPSTR name, int playermax, int flagnum );
void	JoinNetGame							( int sessionnum, LPSTR name );
void	CloseNetGame						( void );

void	PerformChecklistForNetConnections	( void );
void	SetNetConnections					( int index );
void	SetNetConnectionsEx					( int index, LPSTR ipaddress );
void	PerformChecklistForNetSessions		( void );
void	PerformChecklistForNetPlayers		( void );

void	CreatePlayer						( LPSTR playername );
int		CreatePlayerEx						( LPSTR playername );
void	DestroyPlayer						( int playerid );

void	SendNetMsgL							( int playerid, int MessageData );
void	SendNetMsgF							( int playerid, DWORD MessageData );
void	SendNetMsgS							( int playerid, LPSTR pMessageData );

void	SendNetMsgMemblock					( int playerid, int mbi );
void	SendNetMsgMemblockEx				( int playerid, int mbi, int gua );
void	SendNetMsgImage						( int playerid, int imageindex, int gua );
void	SendNetMsgBitmap					( int playerid, int bitmapindex, int gua );
void	SendNetMsgSound						( int playerid, int soundindex, int gua );
void	SendNetMsgMesh						( int playerid, int meshindex, int gua );

void	GetNetMsg							( void );
int		NetMsgInteger						( void );
DWORD	NetMsgFloat							( void );
DWORD	NetMsgString						( DWORD pDestStr );

void	NetMsgMemblock						( int mbi );
void	NetMsgImage							( int imageindex );
void	NetMsgBitmap						( int bitmapindex );
void	NetMsgSound							( int soundindex );
void	NetMsgMesh							( int meshindex );

int		NetMsgExists						( void );
int		NetMsgType							( void );
int		NetMsgPlayerFrom					( void );
int		NetMsgPlayerTo						( void );
int		NetSessionExists					( void );
int		NetSessionLost						( void );
int		NetPlayerCreated					( void );
int		NetPlayerDestroyed					( void );
int		NetSessionIsNowHosting				( void );

int		MagicNetGame						( DWORD lpGameName, DWORD lpPlayerName, int PlayerMax, int FlagNum );
int		NetBufferSize						( void );

void	ConstructorMultiplayer				( void );
void	DestructorMultiplayer				( void );
void	SetErrorHandlerMultiplayer			( LPVOID pErrorHandlerPtr );
void	PassCoreDataMultiplayer				( LPVOID pGlobPtr );
void	RefreshD3DMultiplayer				( int iMode );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int dbDefaultNetGame(char* lpGameName, char* lpPlayerName, int PlayerMax, int FlagNum )
{
	return MagicNetGame ( ( DWORD ) lpGameName, ( DWORD ) lpPlayerName,  PlayerMax,  FlagNum );
}

void dbCreateNetGame(char* gamename, char* name, int playermax)
{
	CreateNetGame(gamename, name, playermax);
}

void dbCreateNetGame(char* gamename, char* name, int playermax, int flagnum)
{
	CreateNetGameEx(gamename, name, playermax, flagnum);
}

void dbJoinNetGame(int sessionnum, char* name)
{
	JoinNetGame ( sessionnum,  name);
}

void dbFreeNetGame(void)
{
	CloseNetGame();
}

void dbCreateNetPlayer(char* playername)
{
	CreatePlayer(playername);
}

int dbCreateNetPlayerEx(char* playername)
{
	return CreatePlayerEx( playername);
}

void dbFreeNetPlayer(int playerid)
{
	DestroyPlayer(playerid);
}

void dbSetNetConnection(int index)
{
	SetNetConnections(index);
}

void dbSetNetConnection(int index, char* ipaddress)
{
	SetNetConnectionsEx( index, ipaddress);
}

void dbPerformChecklistForNetConnections(void)
{
	PerformChecklistForNetConnections();
}

void dbPerformChecklistForNetSessions(void)
{
	PerformChecklistForNetSessions();
}

void dbPerformChecklistForNetPlayers(void)
{
	PerformChecklistForNetPlayers ( );
}

void dbSendNetMsgInteger(int playerid, int MessageData)
{
	SendNetMsgL ( playerid, MessageData);
}

void dbSendNetMsgFloat(int playerid, DWORD MessageData)
{
	SendNetMsgF( playerid, MessageData);
}

void dbSendNetMsgString(int playerid, char* pMessageData)
{
	SendNetMsgS( playerid, pMessageData);
}

void dbSendNetMsgMemblock(int playerid, int mbi)
{
	SendNetMsgMemblock( playerid,  mbi);
}

void dbSendNetMsgMemblock(int playerid, int mbi, int gua)
{
	SendNetMsgMemblockEx(playerid,  mbi,  gua);
}

void dbSendNetMsgImage(int playerid, int imageindex, int gua)
{
	SendNetMsgImage( playerid,  imageindex,  gua);
}

void dbSendNetMsgBitmap(int playerid, int bitmapindex, int gua)
{
	SendNetMsgBitmap( playerid,  bitmapindex,  gua);
}

void dbSendNetMsgSound(int playerid, int soundindex, int gua)
{
	SendNetMsgSound( playerid,  soundindex,  gua);
}

void dbSendNetMsgMesh(int playerid, int meshindex, int gua)
{
	SendNetMsgMesh( playerid,  meshindex,  gua);
}

void dbGetNetMsg(void)
{
	GetNetMsg();
}

int dbNetMsgInteger(void)
{
	return NetMsgInteger();
}

float dbNetMsgFloat(void)
{
	DWORD dwReturn = NetMsgFloat ( );
	
	return *( float* ) &dwReturn;
}

char* dbNetMsgString(void)
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = NetMsgString ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

void dbNetMsgMemblock(int mbi)
{
	NetMsgMemblock( mbi);
}

void dbNetMsgImage(int imageindex)
{
	NetMsgImage(imageindex);
}

void dbNetMsgBitmap(int bitmapindex)
{
	NetMsgBitmap(bitmapindex);
}

void dbNetMsgSound(int soundindex)
{
	NetMsgSound(soundindex);
}

void dbNetMsgMesh(int meshindex)
{
	NetMsgMesh( meshindex);
}

int dbNetGameExists(void)
{
	return NetSessionExists ( );
}

int dbNetMsgExists(void)
{
	return NetMsgExists ( );
}

int dbNetMsgPlayerFrom(void)
{
	return NetMsgPlayerFrom ( );
}

int dbNetMsgPlayerTo(void)
{
	return NetMsgPlayerTo ( );
}

int dbNetMsgType(void)
{
	return NetMsgType ( );
}

int dbNetGameLost(void)
{
	return NetSessionLost ( );
}

int dbNetGameIsNowHosting(void)
{
	return NetSessionIsNowHosting ( );
}

int dbNetBufferSize(void)
{
	return NetBufferSize ( );
}

int dbNetPlayerCreated(void)
{
	return NetPlayerCreated ( );
}

int dbNetPlayerDestroyed(void)
{
	return NetPlayerDestroyed ( );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorMultiplayer ( void )
{
	ConstructorMultiplayer ( );
}

void dbDestructorMultiplayer ( void )
{
	DestructorMultiplayer ( );
}

void dbSetErrorHandlerMultiplayer ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerMultiplayer ( pErrorHandlerPtr );
}

void dbPassCoreDataMultiplayer ( LPVOID pGlobPtr )
{
	PassCoreDataMultiplayer ( pGlobPtr );
}

void dbRefreshD3DMultiplayer ( int iMode )
{
	RefreshD3DMultiplayer ( iMode );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////