#include "database.h"
#include "../log.h"

// constructor
CDatabase::CDatabase( char* server , char* username, char* password, char* database, unsigned int port , MYSQL* mysql)
{
    Server = server;
    Username = username;
    Password = password;
    Database = database;
    Port = port;    
    Mysql = mysql;
    SQLMutex = PTHREAD_MUTEX_INITIALIZER;
    mysql_init( Mysql ); 
}

// deconstructor
CDatabase::~CDatabase( )
{
}

// disconnect from mysql
void CDatabase::Disconnect( )
{
    mysql_close( Mysql );
}

int CDatabase::Connect( )
{
    Log( MSG_INFO, "Connecting to MySQL" );
    if(!mysql_real_connect( Mysql, Server, Username, Password, Database,Port, NULL, 0 ))
    {
        Log( MSG_FATALERROR, "Error connecting to MySQL server: %s\n", mysql_error( Mysql ) );
        return -1;
    }
    if(!QExecute("SET SESSION interactive_timeout=%i", 2678400))
        Log( MSG_WARNING, "Can't change timeout session");
    else
        Log( MSG_INFO, "SQL timeout set to %i seconds", 2678400);
            
    return 0;
}

// reconnect to mysql
int CDatabase::Reconnect( )
{
    if(!mysql_real_connect( Mysql, Server, Username, Password, Database,Port, NULL, 0 ))
    {
            Log( MSG_FATALERROR, "Error reconnecting to MySQL server: %s\n", mysql_error( Mysql ) );
            return -1;
    }
    if(!QExecute("SET SESSION interactive_timeout=%i", 2678400))
        Log( MSG_WARNING, "Can't change timeout session");
    else
        Log( MSG_INFO, "SQL timeout set to %i seconds", 2678400);
        
    return 0;
}


//LMA: Special Update case, returns nb affected rows.
int CDatabase::QExecuteUpdate( char *Format,... )
{
    bool Qfail = true;
    char query[1024];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );    
    Log( MSG_QUERY, query );	
    pthread_mutex_lock( &SQLMutex );
    while(Qfail)
    {
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );
                return -1;
            }
            else Qfail = false;
        }
        else Qfail = false;
    }
    pthread_mutex_unlock( &SQLMutex );            
    return mysql_affected_rows(Mysql);
}

// execute query
bool CDatabase::QExecute( char *Format,... )
{
    bool Qfail = true;
    char query[1024];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );    
    Log( MSG_QUERY, query );	
    pthread_mutex_lock( &SQLMutex );
    while(Qfail)
    {
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }
        else Qfail = false;
    }
    pthread_mutex_unlock( &SQLMutex );            
    return true;    
}

MYSQL_RES* CDatabase::QStore( char *Format, ...)
{
    bool Qfail = true;    
    char query[1024];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );  
    result = NULL;
    Log( MSG_QUERY, query );    
    pthread_mutex_lock( &SQLMutex );  
    while(Qfail)
    {    
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }    
        else Qfail = false;
    }
    result = mysql_store_result( Mysql );
    return result;
}

MYSQL_RES* CDatabase::QUse( char *Format, ...)
{
    bool Qfail = true;    
    char query[1024];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );  
    result = NULL;
    Log( MSG_QUERY, query );    
    pthread_mutex_lock( &SQLMutex );  
    while(Qfail)
    {    
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }    
        else Qfail = false;
    }
    result = mysql_use_result( Mysql );
    return result;
}

void CDatabase::QFree( )
{
    mysql_free_result( result );
    pthread_mutex_unlock( &SQLMutex );
}
