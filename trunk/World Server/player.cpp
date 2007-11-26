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
#include "player.h"
#include "worldserver.h"

CPlayer::CPlayer( CClientSocket* CLIENT )
{  
    client = CLIENT;
    // USED ITEM
    UsedItem = new USEDITEM;
    assert(UsedItem);
    UsedItem->lastRegTime = 0;
    UsedItem->usevalue = 0;
    UsedItem->usetype = 0;
    UsedItem->userate = 0;
    UsedItem->used = 0; 
    // CHARINFO
    CharInfo = new INFO;
    assert(CharInfo);
    memset( &CharInfo->charname, '\0', 17 );
	CharInfo->charid = 0;	    	
    CharInfo->Sex = 0;
    CharInfo->Face = 0;
    CharInfo->Hair = 0;
    CharInfo->Exp = 0;
    CharInfo->Job = 0;
    CharInfo->Zulies = 0;
    CharInfo->Storage_Zulies = 0;
    CharInfo->LastGlobal = 0;
    CharInfo->StatPoints = 0;
    CharInfo->SkillPoints = 0;
    CharInfo->stamina = 0;

    // RIDE
    Ride = new RIDE;
    assert(Ride);
    Ride->Drive = false;
    Ride->Ride = false;
    Ride->charid = 0;
    // TRADE
    Trade = new TRADE;
    assert(Trade);
    for(int i=0;i<10;i++)
        Trade->trade_itemid[i] = 0;
    for(int i=0;i<11;i++)    
        Trade->trade_count[i] = 0;    
    Trade->trade_status = 0;
    Trade->trade_target = 0;
    //PARTY
    Party = new PARTY;
    assert(Party);
    Party->party = NULL;
    Party->IsMaster = true;
    // SHOP    
    Shop = new SHOP;
    assert(Shop);
    Shop->open = false;
    memset( &Shop->name, '\0',64 );
    for(int i=0;i<30;i++)
    {
        ClearItem(Shop->BuyingList[i].item);
        Shop->BuyingList[i].slot = 0;
        Shop->BuyingList[i].count = 0;
        Shop->BuyingList[i].price = 0;
        Shop->SellingList[i].slot = 0;
        Shop->SellingList[i].count = 0;
        Shop->SellingList[i].price = 0;        
    }
    Shop->Buying = 0;
    Shop->Selling = 0;
    Shop->ShopType = 0;
    Shop->mil_shop_time=0;
    
    //bonusxp
    timerxp=0;
    bonusxp=1;
    wait_validation=0;
    once=false;
    
    // SESSION
    Session = new SESSION;
    assert(Session);
	Session->userid = 0;
	memset( &Session->username, '\0', 17 );
	memset( &Session->password, '\0', 33 );	
	Session->accesslevel = 0;
	Session->isLoggedIn = false;
	Session->inGame = false;
	// Inventory / storage
    for(unsigned int i=0;i<MAX_INVENTORY;i++)
        ClearItem( items[i] );
    for(unsigned int i=0;i<MAX_STORAGE;i++)
        ClearItem( storageitems[i] );    
    // Clan
    Clan = new CLAN;
    assert(Clan);
    Clan->clanid = 0;
    Clan->clanrank = 0;
    Clan->grade = 0;
    Clan->logo = 0;
    Clan->back = 0;
    memset( &Clan->clanname, '\0', 17 );
    // ATTRIBUTES
    Attr = new ATTRIBUTES;
    assert(Attr);
    Attr->Str = 0;
    Attr->Dex = 0;
    Attr->Int = 0;
    Attr->Con = 0;
    Attr->Cha = 0;
    Attr->Sen = 0;    
    Attr->Estr = 0;
    Attr->Edex = 0;
    Attr->Eint = 0;
    Attr->Econ = 0;
    Attr->Echa = 0;
    Attr->Esen = 0;   
    
    CharType = TPLAYER;

    questdebug = false;
    Saved = false;
    isInvisibleMode = false;
    Fairy = false;
    hits = 0;
    //FairyTime = 0;
    nstorageitems = 0;
    nsitemmallitems = 0;
    p_skills = 0;
    for(int i=0;i<MAX_SKILL;i++)
    {
        cskills[i].id = 0;
        cskills[i].level = 0;
    }
    for(int i=0;i<MAX_BASICSKILL;i++)
        bskills[i] = 0;
    for(int i=0;i<MAX_QUICKBAR;i++)
        quickbar[i] = 0;
    MyQuest.clear( );
    ActiveQuest = 0;
    lastRegenTime = 0;
    lastSaveTime = clock( );
    firstlogin=clock();     //LMA for fairy
    lastGG = 0;
    VisiblePlayers.clear( );
    VisibleDrops.clear( );
    VisibleMonsters.clear( );
    VisibleNPCs.clear( );
}

CPlayer::~CPlayer( )
{
    if(client!=NULL) 
    {
        delete client;
        client = NULL;
    }
    if(UsedItem!=NULL) delete UsedItem;
    if(CharInfo!=NULL) delete CharInfo;
    if(Ride!=NULL) delete Ride;
    if(Trade!=NULL) delete Trade;
    if(Party!=NULL) delete Party;
    if(Shop!=NULL) delete Shop;
    if(Session!=NULL) delete Session;
    if(Clan!=NULL) delete Clan;
    if(Attr!=NULL) delete Attr;
    MyQuest.clear();
    VisiblePlayers.clear();
    VisibleDrops.clear();
    VisibleMonsters.clear();
    VisibleNPCs.clear();
}

bool CPlayer::IsMonster( )
{
    return false;
}

bool CPlayer::UpdateValues( )
{
    if( Ride->Ride && !Ride->Drive )
    {
        CPlayer* otherclient = GServer->GetClientByCID( Ride->charid, Position->Map );
        if( otherclient!=NULL )
        {            
            if( otherclient->Status->Stance != 0x04 )
            {                
                otherclient->Ride->Drive = false;
                otherclient->Ride->Ride = false;
                otherclient->Ride->charid = 0;                
                Ride->Drive = false;
                Ride->Ride = false;
                Ride->charid = 0;
                Position->destiny = Position->current;  //ONLY ON BACK SEAT
            }
            else
            {
                Position->current = otherclient->Position->current;  //ONLY ON BACK SEAT
                Position->destiny = otherclient->Position->destiny;  //ONLY ON BACK SEAT
                Position->lastMoveTime = otherclient->Position->lastMoveTime;   //ONLY ON BACK SEAT    
                return false;  // will not update the player position                 
            }
        }  
        else
        {      
            Ride->Drive = false;
            Ride->Ride = false;
            Ride->charid = 0;      
            Position->destiny = Position->current;  //RIDER LOST
        }
    }
    
    return true;
}

// Spawn Another User on the Screen
bool CPlayer::SpawnToPlayer( CPlayer* player, CPlayer* otherclient )
{
    BEGINPACKET( pak, 0x793 );
	ADDWORD( pak, clientid);				// USER ID ANYONE?
    ADDFLOAT( pak, Position->current.x*100 );			// POS X
	ADDFLOAT( pak, Position->current.y*100 );			// POS Y
    ADDFLOAT( pak, Position->destiny.x*100 );			// GOING TO X
    ADDFLOAT( pak, Position->destiny.y*100 );			// GOINT TO Y   
    if(Status->Stance == 0x01)
    {
        ADDWORD( pak, 0x000a );     
        ADDWORD( pak, 0x0000 );                
    }
    else
    if(Stats->HP <= 0)
    {
        ADDWORD( pak, 0x0003 );
        ADDWORD( pak, 0x0000 );         
    }
    else
    if(Position->destiny.x != Position->current.y || Position->destiny.y != Position->current.y)
    {
        ADDWORD( pak, 0x0001 );
        ADDWORD( pak, Battle->atktarget );         
    }
    else    
    if(Battle->atktarget!=0)
    {
        ADDWORD( pak, 0x0002 );
        ADDWORD( pak, Battle->atktarget );         
    }    
    else    
    {
        ADDWORD( pak, 0x0001 );                
        ADDWORD( pak, 0x0000 );         
    }
    switch (Status->Stance)
    {
        case WALKING:
           ADDBYTE( pak, 0x00 );
        break;
        case RUNNING:
           ADDBYTE( pak, 0x01 ); 
        break;
        case DRIVING:
           ADDBYTE( pak, 0x02 );    
        break;   
        default:
           ADDBYTE( pak, 0x0b ); 
    }
    ADDWORD( pak, 0x0000 );    
    ADDWORD( pak, 0x0000 );        
    if(otherclient->Party->party==NULL || otherclient->Party->party != player->Party->party || otherclient->Party->party == player->Party->party)
    {
        CMap* map = GServer->MapList.Index[player->Position->Map];
        if(map->allowpvp==1){ADDDWORD(pak, 0x00000051 );} // pvp all vs all
        else
        if(map->allowpvp==2) // pvp group vs group
        {
            if((player->Clan->clanid != otherclient->Clan->clanid) && ((map->id>0) && (map->id<121))) { ADDDWORD(pak, 0x00000051 ); }//Clan War map (pedion)
            else if((player->Clan->clanid == otherclient->Clan->clanid) && ((map->id>0) && (map->id<=100))) { ADDDWORD(pak, 0x00000051 ); }//other pvp group vs group map
            else { ADDDWORD(pak, 0x00000000 ); }
        }
        else { ADDDWORD(pak, 0x00000000 ); }
    }
    else {ADDDWORD(pak, 0x00000000 );}     
    ADDDWORD( pak, GServer->BuildBuffs( this ) );//BUFFS
    ADDBYTE( pak, CharInfo->Sex );					// GENDER
    ADDWORD( pak, Stats->Move_Speed );			// WALK SPEED MAYBE?
    ADDWORD( pak, 0 );								// ??
    ADDBYTE( pak, 0x01 );								// ??
    ADDDWORD( pak, CharInfo->Face );				// FACE TYPE
    ADDDWORD( pak, CharInfo->Hair );			// HAIR TYPE
    ADDWORD( pak, items[2].itemnum );		// CAP
    ADDWORD( pak, GServer->BuildItemRefine( items[2] ) );		// CAP REFINE
    ADDWORD( pak, items[3].itemnum );		// BODY
    ADDWORD( pak, GServer->BuildItemRefine( items[3] ) );		// BODY REFINE
    ADDWORD( pak, items[5].itemnum );		// GLOVES
    ADDWORD( pak, GServer->BuildItemRefine( items[5] )  );		// GLOVES REFINE
    ADDWORD( pak, items[6].itemnum );		// BOOTS
    ADDWORD( pak, GServer->BuildItemRefine( items[6] )  );		// BOOTS REFINE
    ADDWORD( pak, items[1].itemnum );		// FACE
    ADDWORD( pak, GServer->BuildItemRefine( items[1] )  );		// FACE REFINE
    ADDWORD( pak, items[4].itemnum );		// BACK
    ADDWORD( pak, GServer->BuildItemRefine( items[4] )  );		// BACK REFINE 
    ADDWORD( pak, items[7].itemnum );		// WEAPON
    ADDWORD( pak, GServer->BuildItemRefine( items[7] )  );		// WEAPON REFINE
    ADDWORD( pak, items[8].itemnum );		// SUBWEAPON
    ADDWORD( pak, GServer->BuildItemRefine( items[8] )  );		// SUBWEAPON REFINE
    
    /*        
    ADDWORD( pak, ((items[132].itemnum << 5) & 0x3ff) );//arrows
    ADDWORD( pak, ((items[133].itemnum << 5) & 0x3ff) );//bullets
    ADDWORD( pak, ((items[134].itemnum << 5) & 0x3ff) );//cannons    
    */
    //Fix from maximz
    ADDWORD( pak, ((items[132].itemnum << 5) ));//arrows
    ADDWORD( pak, ((items[133].itemnum << 5) ));//bullets
    ADDWORD( pak, ((items[134].itemnum << 5) ));//cannons     
    
    ADDWORD( pak, CharInfo->Job );
    ADDBYTE( pak, Stats->Level );
    ADDWORD( pak, items[135].itemnum);		// CART FRAME
    ADDWORD( pak, GServer->BuildItemRefine( items[135] ) );
    ADDWORD( pak, items[136].itemnum );	// CART ENGINE
    ADDWORD( pak, GServer->BuildItemRefine( items[136] )  );
    ADDWORD( pak, items[137].itemnum );	// CART WHEELS
    ADDWORD( pak, GServer->BuildItemRefine( items[137] )  );
    ADDWORD( pak, items[138].itemnum );	// CART WEAPON
    ADDWORD( pak, GServer->BuildItemRefine( items[138] )  );
    ADDWORD( pak, items[139].itemnum );	// CART ABILITY
    ADDWORD( pak, GServer->BuildItemRefine( items[139] )  );
	ADDWORD( pak, (Stats->HP<=0)?0x0:0xea7b );
    if(Shop->open)
    {
        ADDBYTE( pak, 0x02 );
    }
    else
    {
        ADDBYTE( pak, 0x00 );								
    }          
    ADDBYTE( pak, 0x00);    
    ADDBYTE( pak, 0x00);
    if( Fairy )
    {
        ADDBYTE( pak, 0x61);
    }
    else
    {
        ADDBYTE( pak, 0x00);
    }
	ADDSTRING( pak, CharInfo->charname );
	ADDBYTE( pak, 0x00);
	if(Shop->open)
	{
	   ADDBYTE( pak, Shop->ShopType);
	   ADDBYTE( pak, 0x00);
	   ADDSTRING(pak, Shop->name);
	   ADDBYTE( pak, 0x00);       //LMA 139+
    }
    if(Clan->clanid!=0)
    {
        ADDDWORD( pak, Clan->clanid );
        ADDWORD( pak, Clan->back);
        ADDWORD( pak, Clan->logo);           
        ADDBYTE( pak, Clan->grade);
        ADDBYTE( pak, 0x00);   
        ADDSTRING( pak, Clan->clanname);
        Log(MSG_INFO,"[WS] Clan info in player packet 0x793");
    }
    ADDWORD( pak, 0x0000 );
    player->client->SendPacket(&pak); 
    if( Party->party!=NULL && Party->party == player->Party->party)
    {
        BEGINPACKET( pak, 0x7d5 );
        ADDDWORD   ( pak, CharInfo->charid );
        ADDWORD    ( pak, clientid );
        ADDWORD    ( pak, GetMaxHP( ) );
        ADDWORD    ( pak, Stats->HP );
        ADDDWORD   ( pak, 0x01000000 );
        ADDDWORD   ( pak, 0x0000000f );
        ADDWORD    ( pak, 0x1388 );
        player->client->SendPacket( &pak );
    }          
	if( Ride->Ride )
	{        
        CPlayer* rideclient = GServer->GetClientByCID( Ride->charid, Position->Map );
        if(rideclient==NULL) 
        {
            Ride->Ride = false;
            Ride->Drive= false;
            Ride->charid = 0;              
            return true;
        }
        if( GServer->IsVisible( player, rideclient ) || player->CharInfo->charid == rideclient->CharInfo->charid )
        {
            BEGINPACKET( pak, 0x796 );
            if( Ride->Drive )
            {
                ADDWORD    ( pak, rideclient->clientid );
                ADDFLOAT   ( pak, rideclient->Position->current.x*100 );            
                ADDFLOAT   ( pak, rideclient->Position->current.y*100 );                
            }
            else
            {
                ADDWORD    ( pak, clientid );
                ADDFLOAT   ( pak, Position->current.x*100 );            
                ADDFLOAT   ( pak, Position->current.y*100 );
            }
            ADDWORD    ( pak, 0x0000 );
            player->client->SendPacket( &pak );
                
            RESETPACKET( pak, 0x7dd );
            ADDBYTE    ( pak, 0x02 );
            if( rideclient->Ride->Drive )
            {               
                ADDWORD    ( pak, rideclient->clientid );
                ADDWORD    ( pak, clientid );                   
            }
            else
            {
                ADDWORD    ( pak, clientid );                
                ADDWORD    ( pak, rideclient->clientid );                                              
            }          
            player->client->SendPacket( &pak );                          
        }
    }  
    //Little addition till the nonsense in server sync is fixed.
    //StartAction(NULL, 0, 0, true);  
	return true;
}
