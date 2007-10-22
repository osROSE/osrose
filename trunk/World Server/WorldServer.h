/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://www.osrose.net
    
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
#ifndef __ROSE_SERVERS__
#define __ROSE_SERVERS__
#include "../common/sockets.h"
#include "worldmonster.h"
#include "worldmap.h"
#include "player.h"
#include "character.h"
#include "party.h"
#include "datatypes.h"
#define MAXVISUALRANGE 100
#define MINVISUALRANGE 60
#define ClearItem(i) { i.appraised=false; i.count=0; i.durability=0; i.itemnum=0; i.itemtype=0; i.lifespan=0; i.refine=0; i.socketed=false; i.stats=0; i.gem=0; }

#define WORLD_THREAD 0
#define VISUALITY_THREAD 1
#define SHUTDOWN_THREAD 2

//LMA: Grids
#define NB_MAPS 120        //Nb maps (120 maps, including bad and non existant).
#define NB_GRIDS 72        //nb active grids


//LMA TESTLOOP
extern UINT lma_loop;


// Main loginserver server class
class CWorldServer : public CServerSocket
{
    public:
    	//------------------ MAIN (worldserver.cpp)
    	CWorldServer ( string );
        CClientSocket* CreateClientSocket( );	
    	~CWorldServer( );
    	bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );    
    	void OnClientDisconnect( CClientSocket* thisclient );	
    	void DeleteClientSocket( CClientSocket* thisclient );	
    	bool OnServerReady( );
    	void SpawnMonsters( );	    	
    	void LoadConfigurations( char* ); 
        void LoadCommandLevels( void ); 
        void ServerLoop( );  
        
        //LMA
        //Used for Grids
        CListMap allmaps[NB_MAPS];   //all maps (even those not used)
        CGridMap gridmaps[NB_GRIDS];  //only active maps
        //LMA End
        
      	//------------------ WORLD PROCESS (worldprocess.cpp)  	
      	//bool GiveExp( CMonster* thismon );
      	//LMA BEGIN
      	//20070621-211100
      	//FOR CF
      	bool GiveExp( CMonster* thismon, UINT special_lvl, UINT special_exp);
      	//LMA END
        
        //LMA BEGIN
        //20070623-232300
        bool Ping();        //MySQL Ping
        //LMA END
        
        //------------------ QUEST DATA (quest.cpp)    		    
    	bool pakGiveQuest( CPlayer* thisclient, CPacket* P );	    	
    
    	//------------------ BUFFS (buff.cpp)
        	CBValue GetBuffValue( CSkills* thisskill, CCharacter* character, UINT Evalue, UINT i, UINT up, UINT down, UINT CurrentValue, bool Buff=true, bool Status=false );
            bool CheckABuffs( CSkills* thisskill, CCharacter* character, int Evalue ,int i);
            bool CheckDBuffs( CSkills* thisskill, CCharacter* character, int Evalue ,int i);                        	
        	bool AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue , bool flag);    		
        	bool AddDeBuffs( CSkills* thisskill, CCharacter* character, int Evalue );
        	bool AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue );	
	
    	//------------------ SERVER EXTRAS (extrafunctions.cpp)  
        bool IsValidItem(UINT type, UINT id );
        UINT GetUIntValue( const char* s , void* var=NULL );
        int GetIntValue( const char* s , void* var=NULL );        
        char* GetStrValue( const char* s , void* var=NULL );        
    	bool SendSysMsg( CPlayer* thisclient, string message );
    	UINT RandNumber( UINT init, UINT range, UINT seed=0 );
    	UINT GetColorExp( UINT playerlevel,UINT moblevel, UINT exp );
    	bool CheckInventorySlot( CPlayer* thisclient, int slot );
    	bool pakGMClass( CPlayer* thisclient, char* classid );
        bool pakGMKillInRange( CPlayer* thisclient, int range );  	
    	bool pakGMHide( CPlayer* thisclient, int mode );
    	void SendToVisible( CPacket* pak, CPlayer* thisclient, bool thisclient=true );	
    	void SendToVisible( CPacket* pak, CPlayer* thisclient, CPlayer* xotherclient );		
    	
    	void SendToVisible( CPacket* pak, class CCharacter* character, CDrop* thisdrop=NULL );
            	
    	bool IsMonInCircle( CPlayer* thisclient, CMonster* thismon, float radius );
        bool IsPlayerInCircle( CPlayer* thisclient, CPlayer* otherclient, float radius );
        CUseInfo* GetUseItemInfo(  CPlayer* thisclient, UINT slot );	    	    		
    	bool IsVisible( CPlayer* thisclient, CPlayer* otherclient );		
    	bool IsMonInCircle( fPoint center, fPoint position, float radius );   
    	bool IsVisible( CPlayer* thisclient, CMonster* thismon );	
    	bool IsVisible( CPlayer* thisclient, CDrop* thisdrop );
        CItem GetItemByHeadAndData( unsigned head, unsigned data );    	
    	bool IsVisible( CPlayer* thisclient, CNPC* thisnpc );	
    	void SendToVisible( CPacket* pak, CMonster* thismon, CDrop* thisdrop=NULL );			
    	void SendToVisible( CPacket* pak, CDrop* thisdrop );	
        CPlayer* GetClientByUserName( char *username );	
    	UINT BuildBuffs( CCharacter* player );
    	void CheckForLevelUp ( CPlayer* thisclient );	                 	
        bool isSkillTargetFriendly( CSkills* thisskill );		
    	CRespawnPoint* GetRespawnByID( UINT id );
    	CPlayer* GetClientByCID( UINT id, UINT map=0 );
    	CPlayer* GetClientByCharName( char* name );	
    	CPlayer* GetClientByID( UINT id, UINT map=0 );	
    	CTeleGate* GetTeleGateByID( UINT id );	
    	UINT BuildItemHead( CItem thisitem );
    	UINT BuildItemData( CItem thisitem );	
    	CMonster* GetMonsterByID( UINT id, UINT map );
    	CSpawnArea* GetSpawnArea( UINT id, UINT map=0 );
        bool DeleteSpawn( CSpawnArea* spawn );
        CNPCData* GetNPCDataByID( UINT id );    	
    	void SendToMap( CPacket* pak, int mapid );	
        unsigned BuildItemRefine(CItem thisitem ); 	
        CMDrops* GetDropData( UINT id );     
    	CRespawnPoint* GetRespawnByMap( int id );   
        CSkills* GetSkillByID( UINT id );          		      
    	void ClearClientID( UINT id );
    	CDrop* GetDropByID( UINT id, UINT map=0 );
        CChest* GetChestByID( UINT id );	
    	CNPC* GetNPCByID( UINT id, UINT map=0 );
        CNPC* GetNPCByType( UINT npctype );
        CDrop* GetDrop( CMonster* thismon );	
    	void SendToAll( CPacket* pak );	
    	UINT GetNewClientID( );	   		
    	void DisconnectAll();	
    	CPlayer* GetClientByUserID( UINT userid );   
        UINT GetLevelGhost( UINT map, UINT level );
        UINT GetFairyRange( UINT part );
        UINT GetGridNumber(int mapid, int posx, int posy);    //LMA: maps
        bool SaveAllStorage( CPlayer* thisclient);    //LMA: Saving Storage (all).
        bool GetAllStorage( CPlayer* thisclient);     //LMA: Getting Storage (all).
        bool GetSlotStorage( CPlayer* thisclient,UINT slotnum);   //LMA: refreshing one slot from MySQL storage.
        bool SaveSlotStorage( CPlayer* thisclient,UINT slotnum);  //LMA: Saving one slot into MySQL storage.
        bool GetZulyStorage( CPlayer* thisclient);     //LMA: Get Zuly from Storage (from MySQL)
        bool SaveZulyStorage( CPlayer* thisclient);    //LMA: Save Zuly to Storage (to MySQL)
        void RefreshFairy( );
    
        //------------------ Fairies ---------------------
        void DoFairyStuff( CPlayer* targetclient, int action );
        void DoFairyFree( int fairy );
    
    	//------------------ MATH (extrafunctions.cpp)
    	fPoint RandInCircle(fPoint center, float radius);
    	fPoint RandInPoly(fPoint p[], int pcount);
    	fPoint RandInTriangle(fPoint p1, fPoint p2, fPoint p3);
    	float AreaOfTriangle(fPoint p1, fPoint p2, fPoint p3);
    	float distance( fPoint pos1, fPoint pos2 );
    
    	//------------------ PACKETS (worldpackets.cpp)
    	bool pakItemMall( CPlayer* thisclient, CPacket* P );
    	bool pakWeight( CPlayer* thisclient, CPacket* P );
    	bool pakPrintscreen( CPlayer* thisclient, CPacket* P );
    	bool pakSpawnChar( CPlayer* thisclient, CPlayer* otherclient );	
        bool pakModifiedItemDone( CPlayer* thisclient, CPacket* P );	
    	bool pakSpawnDrop( CPlayer* thisclient, CDrop* thisdrop );        	    
        bool pakChangeStorage( CPlayer* thisclient, CPacket* P );
        bool pakModifiedItem( CPlayer* thisclient, CPacket* P );
        bool pakPartyManager( CPlayer* thisclient, CPacket* P ); 
        bool pakLevelUpSkill( CPlayer *thisclient, CPacket* P ); 
    	bool pakChangeStance( CPlayer* thisclient, CPacket* P );   
        bool pakCharDSClient( CPlayer* thisclient, CPacket* P );          
    	bool pakChangeEquip( CPlayer* thisclient, CPacket* P );    
    	bool pakStartAttack( CPlayer* thisclient, CPacket* P );
       	bool pakSpawnNPC( CPlayer* thisclient, CNPC* thisnpc );          
        bool pakRideRequest( CPlayer* thisclient, CPacket* P );     
        bool pakPartyActions( CPlayer* thisclient, CPacket* P );      
        bool pakPartyOption( CPlayer* thisclient, CPacket* P ); 
    	bool pakTradeAction( CPlayer* thisclient, CPacket* P ); 
    	bool pakChangeCart( CPlayer* thisclient, CPacket* P );    	
        bool pakSkillSelf ( CPlayer* thisclient, CPacket* P );    
    	bool pakDoIdentify( CPlayer *thisclient, CPacket *P );    
    	bool pakNormalChat( CPlayer* thisclient, CPacket* P );		       
    	bool pakCharSelect( CPlayer* thisclient, CPacket* P );
    	bool pakStartSkill( CPlayer* thisclient, CPacket* P );	       
        bool pakRepairItem( CPlayer* thisclient, CPacket* P );  
    	bool pakMoveSkill( CPlayer* thisclient, CPacket* P );    		
        bool pakCloseShop( CPlayer* thisclient, CPacket* P );          
        bool pakStoreZuly( CPlayer* thisclient, CPacket* P );    
        bool pakPartyChat( CPlayer* thisclient, CPacket* P );
    	bool pakGameGuard( CPlayer* thisclient, CPacket* P );	
        bool pakOpenShop( CPlayer* thisclient, CPacket* P );
        bool pakShowShop( CPlayer* thisclient, CPacket* P );    
        bool pakSellShop( CPlayer* thisclient, CPacket* P );    
    	bool pakMoveChar( CPlayer* thisclient, CPacket* P );
    	bool pakStopChar( CPlayer* thisclient, CPacket* P );           
    	bool pakPickDrop( CPlayer* thisclient, CPacket* P );          
    	bool pakAddStats( CPlayer* thisclient, CPacket* P );	            
        bool pakShowHeal( CPlayer* thisclient, CPacket* P );   
        bool pakSkillAOE( CPlayer* thisclient, CPacket* P );
        bool pakidentify( CPlayer* thisclient, CPacket* P ); 
    	bool pakUserDied( CPlayer* thisclient, CPacket* P );
    	bool pakTradeAdd( CPlayer* thisclient, CPacket* P );
        bool pakUseItem ( CPlayer* thisclient, CPacket* P );     
    	bool pakWhisper( CPlayer* thisclient, CPacket* P );	
    	bool pakEquipABC(CPlayer* thisclient, CPacket *P );	
        bool pakStorage( CPlayer* thisclient, CPacket* P );		
        bool pakBuyShop( CPlayer* thisclient, CPacket* P );        
    	bool pakDoEmote( CPlayer* thisclient, CPacket* P );       
    	bool pakNPCBuy( CPlayer* thisclient, CPacket* P );  
    	bool pakDoDrop( CPlayer* thisclient, CPacket* P );                   
    	bool pakShout( CPlayer* thisclient, CPacket* P );        
    	bool pakCraft( CPlayer* thisclient, CPacket* P );   	
    	bool pakDoID( CPlayer* thisclient, CPacket* P );	
    	bool pakGate( CPlayer* thisclient, CPacket* P );	
    	bool pakChangeRespawn( CPlayer* thisclient, CPacket* P );
        bool pakExit( CPlayer* thisclient, CPacket* P );
    	bool pakPing( CPlayer* thisclient, CPacket* P );	          	
      	bool SendLevelUPtoChar(CPlayer* thisclient ); 
       bool pakRepairTool( CPlayer* thisclient, CPacket* P );       
        void pakPlayer( CPlayer *thisclient );
        void pakInventory( CPlayer *thisclient );
        void pakQuestData( CPlayer *thisclient );	
    	void pakClearUser( CPlayer* thisclient );                                                                                                       
        bool LearnSkill( CPlayer* thisclient, UINT skill );
            
        //-------------------------- Clan functions
        bool pakClanManager ( CPlayer* thisclient, CPacket* P );    
        bool pakCreateClan ( CPlayer* thisclient, CPacket* P );
    
    	//------------------ GM COMMANDS (gmcmds.cpp)
    	bool pakGMMoveTo( CPlayer* thisclient, fPoint position ); 
    	bool pakGMTeleAllHere( CPlayer* thisclient ); 
    	bool GMShowTargetInfo( CPlayer* thisclient );
    	bool pakGMServerInfo( CPlayer* thisclient );
    	bool pakGMHeal( CPlayer* thisclient );
    	bool pakGMStat( CPlayer* thisclient, char* statname, int statvalue );
        bool pakGMItemtoplayer(CPlayer* thisclient, char* name , UINT itemid , UINT itemtype , UINT itemamount , UINT itemrefine , UINT itemls, UINT itemstats , UINT itemsocket) ;
        bool pakGMItem( CPlayer* thisclient, UINT itemid , UINT itemtype , UINT itemamount , UINT itemrefine , UINT itemls, UINT itemstats , UINT itemsocket );
        //LMA: for Quests
        bool pakGMItemQuest( CPlayer* thisclient, UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket, char buffer2[200] );
        bool pakGMTeleOtherPlayer( CPlayer *thisclient, char* name, int map, float x, float y );    
        bool pakGMZuly( CPlayer* thisclient, int mode, int amount, char* charname );
        bool pakGMEventType(CPlayer* thisclient, int npctype, int dialog, int type); //Event
    	bool pakGMTele( CPlayer* thisclient, int map, float x, float y );
    	bool pakGMMon( CPlayer* thisclient, int montype, int moncount );    
        bool pakGMZulygive(CPlayer* thisclient, char* name, int zuly);    
        bool pakGMFairyto(CPlayer* thisclient, char* name, int mode);
        bool pakGMClanRewardPoints(CPlayer* thisclient, char* name, int points);    //reward points
        bool pakGMManageFairy(CPlayer* thisclient, int mode); 
        bool pakGMChangeFairyWait(CPlayer* thisclient, int newvalue);
        bool pakGMChangeFairyStay(CPlayer* thisclient, int newvalue); 
        bool pakGMChangeFairyTestMode(CPlayer* thisclient, int mode); 
        bool pakGMTelePlayerHere( CPlayer* thisclient, char* name );
        bool pakGMAllSkill ( CPlayer* thisclient, char* name); // by crashinside
        bool pakGMDelSkills ( CPlayer* thisclient, char* name); // by rl2171
        bool pakGMReborn( CPlayer* thisclient); //Reborn by Core
        bool pakGMLevel( CPlayer* thisclient, int level , char* name);    
        bool pakGMTeleToPlayer( CPlayer* thisclient, char* name );    
        bool pakGMDoEmote( CPlayer* thisclient, int emotionid );      
    	bool pakGMCommand( CPlayer* thisclient, CPacket* P );
        bool ReloadMobSpawn( CPlayer* thisclient, int id );    	
        bool pakGMKick( CPlayer* thisclient, char* name );    
    	bool pakGMAnn( CPlayer* thisclient, CPacket *P );    
        bool pakGMBan( CPlayer* thisclient, char* name );   
        bool pakGMInfo(CPlayer* thisclient, char* name);    
        bool pakGMNpc(CPlayer* thisclient, int npcid, int dialogid);
      	bool pakGMGotomap( CPlayer* thisclient, int map );
      	bool pakGMMute( CPlayer* thisclient, char* name, int time);
      	bool pakGMPartylvl( CPlayer* partyclient, int level );
      	bool pakGMChangePlayerDmg(CPlayer* thisclient, int rate);
      	bool pakGMChangeMonsterDmg(CPlayer* thisclient, int rate);
      	bool pakGMChangeCfmode(CPlayer* thisclient, int mode);
      	bool pakGMWhoAttacksMe(CPlayer* thisclient);
      	bool pakGMChangeAtkSpeedModif(CPlayer* thisclient, int modif);
      	bool pakGMChangeHitDelayModif(CPlayer* thisclient, int modif);
      	bool pakGMChangeMSpeedModif(CPlayer* thisclient, int modif);
        bool pakGMBuff(CPlayer* thisClient, int strength); // by Drakia
        bool pakGMDebuff(CPlayer* thisClient); // by Drakia
        bool pakGMGiveBuff(CPlayer* thisClient, CPlayer* targetClient, int skillID, int strength); // by Drakia
        bool pakGMMaxStats(CPlayer* thisClient);
    
    	//------------------ CHAR SERVER COMMANDS (worldpackets.cpp)
    	bool TeleportTo ( CPlayer* thisclient, int map, fPoint position );	
    	bool pakCSCharSelect( CPlayer* thisclient, CPacket* P );	
    	bool pakCSReady( CPlayer* thisclient, CPacket* P );
	
    	//----------------- Server StartUp Functions (startup.cpp)
    	bool LoadZoneData( );
    	bool LoadGrids( );         //LMA: maps
    	bool LoadConsItem( );
    	bool LoadSellData( );
    	bool LoadProductItem( );
    	bool LoadPatItem( );
    	bool LoadNaturalItem( );
    	bool LoadJemItem( );
    	bool LoadEquip( );
    	bool LoadItemStats( );
        bool LoadTeleGateData( );
        bool LoadMonsterSpawn( );
        bool LoadRespawnData( );
        bool LoadQuestData( );
        bool LoadDropsData( );
        bool LoadChestData( );
        bool LoadSkillData( );
        bool LoadMonsters( );
        bool LoadNPCData( );
        bool LoadUpgrade( );    
        bool LoadNPCs( );
        bool CleanConnectedList( );
    
        CQuest* GetQuestByID( unsigned long int id );
        CQuest* GetQuestByQuestID( unsigned long int questid );
        CQuest* GetQuestByFinalID( unsigned long int questid );
        bool DoQuestScript( CPlayer* thisclient, CQuest* thisquest );
        QUESTS* GetQuestByItemID( unsigned long int itemid );    
    
        // Server Functions 
        bool SendPM( CPlayer* thisclient, char msg[200] );
        bool SendGlobalMSG( CPlayer* thisclient, char msg[200] );
        UINT GetMaxPartyExp( UINT partylevel );
        bool DoSkillScript( CCharacter* character, CSkills* thisskill );
        
        bool AddParty( CParty* );
        bool RemoveParty( CParty* );

    	UINT				    ClientIDList[0x10000];	// Clients List
    	SOCKET					csock;					// Socket for accessing the char server
    	char*					cct;					// Encryption table for char server
    	
    	vector<CTeleGate*>		TeleGateList;			// Telegates List	
    	
        vector<CQuest*>         QuestList;              // Quest List	
        vector<CSkills*>        SkillList;              // Skills List
        vector<CMDrops*>        MDropList;              // Drops List	
        vector<CNPCData*>       NPCData;                // NPC/Mobs List
        vector<CParty*>         PartyList;              // Party List
        vector<CFairy*>         FairyList;              // Fairy List
        vector<CChest*>         ChestList;              // Chest List
        
        CItemStas               StatsList[500];
        UINT                    upgrade[10];
        CEquipList              EquipList[10];
        CJemList                JemList;
        CNaturalList            NaturalList;
        CPatList                PatList;
        CProductList            ProductList;
        CSellList               SellList;
        CUseList                UseList;
        CMapList                MapList;
    	
    	clock_t				   lastServerStep;			// Last Update
    	clock_t                LastUpdateTime;          // Store the last update World Time
    	bool                   ServerOnline;	
        string filename;
    
        pthread_t WorldThread[3];
        pthread_t MapThread[200];
        pthread_attr_t at;    
        pthread_mutex_t MapMutex,SQLMutex,PlayerMutex;     
        
        CDatabase* DB;  
        
        //Test thingies
        unsigned ATTK_SPEED_MODIF;
        unsigned HIT_DELAY_MODIF;
        unsigned MOVE_SPEED_MODIF;
};
extern class CWorldServer* GServer;

PVOID VisibilityProcess( PVOID TS );
PVOID MapProcess( PVOID TS );
PVOID WorldProcess( PVOID TS );
PVOID ShutdownServer( PVOID TS );

void StartSignal( );
void StopSignal( );
void HandleSignal( int num );
// -----------------------------------------------------------------------------------------

#endif
