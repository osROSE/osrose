/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://osroseon.to.md
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source        
*/
#include "sockets.h"

// Constructor
CServerSocket::CServerSocket( )
{
    ConnectedClients = 0;
}

// Destructor
CServerSocket::~CServerSocket( )
{
    
}

// Start up our server
bool CServerSocket::StartServer( )
{
	struct sockaddr_in ain;

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if (sock == INVALID_SOCKET) 
    {
		Log( MSG_FATALERROR, "Could not create a socket" );
		return false;
	}
    int optval = 1;
    if(setsockopt(sock, SOL_SOCKET,SO_KEEPALIVE,(const char*)&optval,sizeof(optval))==-1)
    {
        Log(MSG_ERROR, "setsockopt:SO_KEEPALIVE" );        
    }    
    setsockopt(sock, IPPROTO_TCP,TCP_NODELAY,(const char*)&optval,sizeof(optval));
	ain.sin_family		= AF_INET;
	ain.sin_addr.s_addr	= INADDR_ANY;
	ain.sin_port		= htons( port );
	memset(&(ain.sin_zero), '\0', 8);
	if ( bind( sock, (const sockaddr*)&ain, sizeof( struct sockaddr ) ) ) 
    {
		Log( MSG_FATALERROR, "Could not bind socket" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		return false;
	}

	if ( listen( sock, SOMAXCONN ) == -1 ) 
    {
		Log( MSG_FATALERROR, "Could not listen on socket" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		return false;
	}

	isActive = true;

	if ( !this->OnServerReady( ) )
    {
		Log( MSG_FATALERROR, "Server could not start" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		isActive = false;
		return false;
	}
	Log( MSG_INFO, "Server started on port %i and is listening.", port );
	ServerLoop( );
	// Nothing past here is ever really called
	OnServerDie( );
	closesocket( sock );

	return true;
}

// Server is started, lets run our loop :D
void CServerSocket::ServerLoop( )
{
	fd_set		fds;
	int			activity;
    maxfd = 0;
	sockaddr_in	ClientInfo;
	SOCKET		NewSocket;
	timeval		timeout;
	maxfd = sock;
	OnServerStep();
	
	//LMA BEGIN
	//MySQL Ping (every hour)
	//20070623, 221000
	UINT time_last_ping=clock();
	UINT delay_ping=3600000;
	//LMA END
	
	do
	{	
        //LMA BEGIN
        //MySQL Ping
        //20070623, 221000
        UINT etime = (UINT)round((clock( ) - time_last_ping));
        if(etime>=delay_ping)
        {
            time_last_ping=clock();
            Ping();           
        }
        //LMA END
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;        
        NewSocket = INVALID_SOCKET;
        FD_ZERO( &fds );    
		if(!Config.usethreads)
    	   FillFDS( &fds );    	
		FD_SET( sock, &fds );
		activity = select( maxfd+1, &fds, NULL, NULL, &timeout );
		if ( activity == 0 ) continue;
		if ( activity < 0 && errno != EINTR ) 
        {
			#ifdef _WIN32
			Log( MSG_ERROR, "Select command failed. Error #%i", WSAGetLastError() );
			#else
			Log( MSG_ERROR, "Select command failed. Error #%i", errno );			
			#endif
			isActive = false;
		}
		if ( FD_ISSET( sock, &fds ) && ConnectedClients < 1024 )
        {
			int clientinfolen = sizeof( sockaddr_in );	
            #ifdef _WIN32
       		NewSocket = accept( sock, (sockaddr*)&ClientInfo, (int*)&clientinfolen );
       		#else
    		NewSocket = accept( sock, (sockaddr*)&ClientInfo, (socklen_t*)&clientinfolen );
            #endif			
			// TODO: check if server is full
			if (NewSocket != INVALID_SOCKET)
				AddUser( NewSocket, &ClientInfo );
			else
			{
			    #ifdef _WIN32
			    Log( MSG_ERROR, "Error accepting socket: %i", WSAGetLastError() );
			    #else
			    Log( MSG_ERROR, "Error accepting socket: %i", errno );			    
			    #endif
            }
		}
		if(!Config.usethreads)
    		HandleClients( &fds );
	} while( isActive );
}

//LMA BEGIN
//MySQL Ping
//20070623, 224500
bool CServerSocket::Ping()
{
     return true;
}
//LMA END

// Fills out an FDS for the server
void CServerSocket::FillFDS( fd_set* fds )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{		
        CClientSocket* client = ClientList.at( i );
        if(client->isActive)
        {
		      FD_SET( (unsigned)client->sock, fds );
		      if(client->sock>maxfd)
		          maxfd=client->sock;          		
        }
        else
        {
            DisconnectClient( client );                                       
        }
	}    	
}

// Handle all our clients
void CServerSocket::HandleClients( fd_set* fds )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{
        CClientSocket* client = ClientList.at( i );
        if(!client->isActive)
            continue;
		if(FD_ISSET( client->sock, fds ))
		{
			if(!client->ReceiveData( ) )
			{
                client->isActive = false;
                DisconnectClient( client ); 
			}
		}
	}  
}

// Add a new user to our server
void CServerSocket::AddUser( SOCKET sock, sockaddr_in* ClientInfo )
{
    ConnectedClients++;
	CClientSocket* thisclient = this->CreateClientSocket( );
	if (thisclient==NULL) 
    { 
		closesocket( thisclient->sock );
		if (thisclient!=0) delete thisclient;
		thisclient=0;
		return;
	}	
	thisclient->GS = this;
	thisclient->sock = sock;
	thisclient->isActive = true;
	
	if (!OnClientConnect( thisclient )) {
		closesocket( thisclient->sock );
		if (thisclient!=0) delete thisclient;
		thisclient=0;
		return;
	}
	Log( MSG_INFO, "[%i]User connected from %s", thisclient->sock, inet_ntoa( ClientInfo->sin_addr ) );
	thisclient->ClientIP = "";
    thisclient->ClientIP = inet_ntoa( ClientInfo->sin_addr );
    char *tmp;
    memset(&thisclient->ClientSubNet, '\0', 12 );
    sprintf(thisclient->ClientSubNet, "%i.%i.%i",
           (ClientInfo->sin_addr.s_addr )&0xFF, (ClientInfo->sin_addr.s_addr>>8 )&0xFF,
           (ClientInfo->sin_addr.s_addr>>16)&0xFF);	
	ClientList.push_back( thisclient );
	if(Config.usethreads)
	{
        pthread_create( &threads[sock], NULL, ClientMainThread, (PVOID)thisclient);     	   
    }
    memcpy( &thisclient->clientinfo, ClientInfo, sizeof(struct sockaddr_in));
}

// Disconnect our user
void CServerSocket::DisconnectClient( CClientSocket* thisclient )
{
    ConnectedClients--;
	OnClientDisconnect( thisclient );
	closesocket( thisclient->sock );
	thisclient->isActive = false;
  	thisclient->sock = INVALID_SOCKET;
    for(UINT i=0;i<ClientList.size( );i++)
    {
        CClientSocket* client = ClientList.at( i );
        if( client == thisclient )
        {
            ClientList.erase( ClientList.begin( ) + i );
            break;
        }
    }		
	DeleteClientSocket( thisclient );
}

// This function creates an appropriate client socket
CClientSocket* CServerSocket::CreateClientSocket ( )
{
	CClientSocket* thisclient;
    thisclient = new (nothrow) CClientSocket( );	
	return thisclient;
}

// This function deletes an old client socket
void CServerSocket::DeleteClientSocket( CClientSocket* thisclient )
{
	Log( MSG_INFO, "User disconnected" );
//    Log( MSG_INFO, " %s : disconnected" , thisclient->CharInfo->charname);
    
	delete thisclient;
}

// This function is called just before proccessing clients
void CServerSocket::OnServerStep( )
{
}

// This function is called just before the server starts
bool CServerSocket::OnServerReady( )
{
	return true;
}

// This function is called just before the server dies
void CServerSocket::OnServerDie( )
{
	// DOESNT WORK - DAMN CONSOLE APPS
}

// This function is called, if a client receives data
bool CServerSocket::OnReceivePacket( CClientSocket* thisclient, CPacket *P )
{
	return true;
}

// This function is called, if a client connects
bool CServerSocket::OnClientConnect( CClientSocket* thisclient )
{
	return true;
}

// This function is called, if a client disconnects
void CServerSocket::OnClientDisconnect( CClientSocket* thisclient )
{
}
