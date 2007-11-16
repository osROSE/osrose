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

// Returns the amount of exp that is needed for the next level
UINT CPlayer::GetLevelEXP( )
{
	if (Stats->Level <= 15)       return (unsigned int)( ( Stats->Level + 10 )  * ( Stats->Level + 5 )  * ( Stats->Level + 3 ) * 0.7 );
	else if (Stats->Level <= 50)  return (unsigned int)( ( Stats->Level - 5 )   * ( Stats->Level + 2 )  * ( Stats->Level + 2 ) * 2.2 );
	else if (Stats->Level <= 100) return (unsigned int)( ( Stats->Level - 38 )  * ( Stats->Level - 5 )  * ( Stats->Level + 2 ) * 9 );
	else if (Stats->Level <= 139) return (unsigned int)( ( Stats->Level + 220 ) * ( Stats->Level + 34 ) * ( Stats->Level + 27 ) );
	else                     return (unsigned int)( ( Stats->Level - 126 ) * ( Stats->Level - 15 ) * ( Stats->Level + 7 ) * 41 );
}

// check if player can level up
bool CPlayer::CheckPlayerLevelUP( )
{
	if (CharInfo->Exp >= GetLevelEXP())
    {
	    CharInfo->Exp -= GetLevelEXP();
	    Stats->Level++;
	    Stats->HP = GetMaxHP( );
	    Stats->MP = GetMaxHP( );
	    CharInfo->StatPoints += 10 + (Stats->Level/2);
	    if(Stats->Level>=10)
        CharInfo->SkillPoints += 1;

		BEGINPACKET( pak, 0x79e );
		ADDWORD( pak, clientid );
		ADDWORD( pak, Stats->Level );
		ADDDWORD( pak, CharInfo->Exp );
		ADDWORD( pak, CharInfo->StatPoints );
		ADDWORD( pak, CharInfo->SkillPoints );
		client->SendPacket( &pak );

		RESETPACKET( pak, 0x79e );
		ADDWORD( pak, clientid );
		GServer->SendToVisible( &pak, this );
		SetStats( );
            //SendLevelUPtoChar(this);
	}        
	return true;
}


// Send a PM to client with user information
bool CPlayer::GetPlayerInfo( )
{
    char text[50];    
    sprintf(text,"Attack: %i | Critical: %i",Stats->Attack_Power, Stats->Critical );
	BEGINPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                    
	client->SendPacket(&pak);    	
    sprintf(text,"Defense: %i | Magic Defense: %i",Stats->Defense, Stats->Magic_Defense);
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                 
	client->SendPacket(&pak);
    sprintf(text,"Accury: %i | Dodge: %i",Stats->Accury,Stats->Dodge );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                 
	client->SendPacket(&pak); 	       	    		
    sprintf(text,"aspeed: %i | mspeed: %i",Stats->Attack_Speed,Stats->Move_Speed );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );         
	client->SendPacket(&pak);		    	
    sprintf(text,"HP: %i/%i , MP: %i/%i",Stats->HP,Stats->MaxHP,Stats->MP,Stats->MaxMP);
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                  
	client->SendPacket(&pak);  
    sprintf(text,"Position[%i]: (%.0f,%.0f)",Position->Map,Position->current.x,Position->current.y);
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                   
	client->SendPacket(&pak); 
    sprintf(text,"ClientID: %u | CharID: %u", clientid, CharInfo->charid );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                   
	client->SendPacket(&pak);       
    sprintf(text,"inGame: %i | Logged: %i", Session->inGame, Session->isLoggedIn );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                   
	client->SendPacket(&pak);   
    sprintf(text,"ClanName[%u]: %s | ClanGrade: %i | ClanRank: %i", Clan->clanid, Clan->clanname, Clan->grade, Clan->clanrank );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[GM]PlayerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, text );
	ADDBYTE( pak, 0 );                   
	client->SendPacket(&pak);            
    return true;
}

// clearn player lists
bool CPlayer::CleanPlayerVector( )
{
    CMap* map = GServer->MapList.Index[Position->Map];
    VisiblePlayers.clear();
	VisibleMonsters.clear();
	VisibleDrops.clear();
	VisibleNPCs.clear();
	return true;
}

//LMA BEGIN
//20070621-211100/outdated?
//we clear and "regenerate" a monster...
bool CPlayer::ForceRefreshMonster(bool refresh_all, UINT monster_id)
{
     CMap* map = GServer->MapList.Index[Position->Map];

    // Monsters
    for(UINT i=0;i<map->MonsterList.size();i++)
    {
        CMonster* thismon = map->MonsterList.at( i );
          if (thismon==NULL)
          {
             return false;
          }
          
          if (!refresh_all && (thismon->clientid!=monster_id))
          {
              continue;
          }
          
        //here we are only interested to respawn guys who kick our ass being invisible :)
		float distance = GServer->distance ( this->Position->current, thismon->Position->current );        		
		if ( GServer->IsVisible( this, thismon ) ) 
        {

			if (distance < MAXVISUALRANGE ) 
			{
                //we clear it
                ClearObject( thismon->clientid );
                //and respawn it
                thismon->SpawnMonster(this, thismon );
                //the visibility list will be updated in time automatically.
                //if the monster kicks you, it's because it's already too near ;)
                
              if (!refresh_all)
              {
                  return true;
              }
              
            }
            
		}      

	}    
	
	
	return true;
}
//LMA END

// update visibility list
bool CPlayer::VisiblityList( )
{
	std::vector<CPlayer*>	newVisiblePlayers;
	std::vector<CDrop*>			newVisibleDrops;
	
	//LMATEST
	//std::vector<CMonster*>		newVisibleMonsters;
	std::vector<unsigned int>        newVisibleMonsters;
	
	
	
	std::vector<CNPC*>			newVisibleNPCs;  
	// Clients
	CMap* map = GServer->MapList.Index[Position->Map];
    for(UINT i=0;i<map->PlayerList.size();i++)
    {
        CPlayer* otherclient = map->PlayerList.at(i);
		if ( this==otherclient || !otherclient->Session->inGame) 
		{
            continue;    
        }
		float distance = GServer->distance( this->Position->current, otherclient->Position->current );
		if ( GServer->IsVisible( this, otherclient ) ) 
        {
			if ( distance < MAXVISUALRANGE && !otherclient->isInvisibleMode )
			{
				newVisiblePlayers.push_back( otherclient );
            }
			else
			{
				ClearObject( otherclient->clientid );
            }
		}
        else
        {
			if ( distance < MINVISUALRANGE && !otherclient->isInvisibleMode ) 
            {
			    newVisiblePlayers.push_back( otherclient );
				otherclient->SpawnToPlayer(this, otherclient);
			}
        }
	}    
    // Monsters
    for(UINT i=0;i<map->MonsterList.size();i++)
    {
        CMonster* thismon = map->MonsterList.at( i );
		float distance = GServer->distance ( this->Position->current, thismon->Position->current );        		
		if ( GServer->IsVisible( this, thismon ) ) 
        {			
			if (distance < MAXVISUALRANGE ) 
			{
                //LMATEST
                //newVisibleMonsters.push_back( thismon );
                newVisibleMonsters.push_back( thismon->clientid );
            }
			else
			{
				ClearObject( thismon->clientid );
            }
		}
        else
        {
			if ( distance< MINVISUALRANGE ) 
            {
                 //LMATEST
				//newVisibleMonsters.push_back( thismon );
				newVisibleMonsters.push_back( thismon->clientid );
				thismon->SpawnMonster(this, thismon );
            }
		}
	}    
	// Drops
	for(unsigned i=0; i<map->DropsList.size(); i++) 
    {
		CDrop* thisdrop = map->DropsList.at(i);
		float distance = GServer->distance( this->Position->current, thisdrop->pos );
		if ( GServer->IsVisible( this, thisdrop ) ) 
        {
			if ( distance< MAXVISUALRANGE ) 
			{
				newVisibleDrops.push_back( thisdrop );
            }
			else
			{
				this->ClearObject( thisdrop->clientid );
            }
		}
        else
        {
			if ( distance < MINVISUALRANGE ) 
            {
				newVisibleDrops.push_back( thisdrop );
				GServer->pakSpawnDrop( this, thisdrop );
			}
		}
	}      
	// Npcs
	for(unsigned i=0; i<map->NPCList.size(); i++) 
    {
		CNPC* thisnpc = map->NPCList.at(i);
		float distance = GServer->distance( this->Position->current, thisnpc->pos );
		if ( GServer->IsVisible( this, thisnpc ) ) 
        {
			if ( distance < MAXVISUALRANGE ) 
            {
				newVisibleNPCs.push_back( thisnpc );
            }
			else
			{
        		this->ClearObject( thisnpc->clientid );
            }
		}
        else
        {
			if ( distance < MINVISUALRANGE ) 
            {
				newVisibleNPCs.push_back( thisnpc );
				GServer->pakSpawnNPC( this, thisnpc );
			}
		}
	} 
	VisiblePlayers.clear();
	VisibleDrops.clear();
	VisibleMonsters.clear();
	VisibleNPCs.clear();
	VisiblePlayers = newVisiblePlayers;
	VisibleDrops = newVisibleDrops;
	VisibleMonsters = newVisibleMonsters;
	VisibleNPCs = newVisibleNPCs; 	
    return true;

}


// Returns a free slot in the inventory (0xffff if is full)
UINT CPlayer::GetNewItemSlot( CItem thisitem )
{
    UINT tabsize = 30;
    UINT itemtab = 0;
    switch(thisitem.itemtype)
    {
        case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9://equip
            itemtab=0;
        break;
        case 10://consumables
            itemtab=1;
        break;
        case 11:case 12://etc
            itemtab=2;            
        break;  
        case 14://pat
            itemtab=3;
        break;
        default:
            Log(MSG_WARNING,"unknown itemtype %i", thisitem.itemtype);
            return 0xffff;
         break;      
    }
    for(int i=0;i<30;i++)
    {
        UINT slot=12;
        slot += (tabsize*itemtab)+i;
        switch(itemtab)
        {
            case 0:case 3://equip and pat
            {
                if(items[slot].itemnum==0 && items[slot].count<1)
                    return slot;
            }
            break;
            case 1:case 2://consumable and etc - updated by Core
            {
               if((items[slot].itemnum == thisitem.itemnum && items[slot].itemtype == thisitem.itemtype && items[slot].count<999)
                    ||(items[slot].itemnum==0 && items[slot].count<1))
                    return slot;            }
            break;                                 
        }
    }	
	return 0xffff;
}

// Returns a free slot in the storage (0xffff if is full)
UINT CPlayer::GetNewStorageItemSlot( CItem thisitem )
{
     //LMA: previous algo
     /*
    for(UINT i=0;i<160;i++)
    {
        if(storageitems[i].itemtype == 0)
            return i;
    }
    */
    
    //LMA: New one, we try to see if we can stack some items...
    //non stackable items, so a new slot will be enough...
    if(!(thisitem.itemtype>9 && thisitem.itemtype<14))
    {
        for(UINT i=0;i<160;i++)
        {
            if(storageitems[i].itemtype == 0)
                return i;
        }   
        
        //no place...
        return 0xffff;                     
    }
    
    //stackables items.
    int free_slot=-1;
     for(UINT i=0;i<160;i++)
    { 
        if(storageitems[i].itemtype == 0)
        {
           if (free_slot==-1)
               free_slot=i;
           continue;
        }
        
        //same item and enough place? Let's use this slot :)
        if ((storageitems[i].itemtype==thisitem.itemtype)&&(storageitems[i].itemnum==thisitem.itemnum)&&(storageitems[i].count+thisitem.count<=999))
           return i;        
    }       
    
    if (free_slot!=-1)
       return free_slot;
    
	return 0xffff;
}

// Erase a object from the user
bool CPlayer::ClearObject( unsigned int otherclientid )
{
	BEGINPACKET( pak, 0x794 );
	ADDWORD( pak, otherclientid );
	client->SendPacket( &pak );           	
	return true;
}

// Clean the player values
void CPlayer::RestartPlayerVal( )
{
    ClearBattle( Battle );
    Shop->open = false;
    Trade->trade_target = 0;
    Trade->trade_status = 0;
}

//LMA
//Force refresh of actual HP / MP Hp/Mp jumping bug.
bool CPlayer::RefreshHPMP()
{
    clock_t etime = clock() - lastShowTime;
    if ( etime < CLOCKS_PER_SEC )
        return true;

    BEGINPACKET( pak, 0x7ec );
    ADDWORD    ( pak, Stats->HP );
    ADDWORD    ( pak, Stats->MP );
    client->SendPacket ( &pak );

    if (Party->party != NULL)
    {
        BEGINPACKET( pak, 0x7d5 );
        ADDDWORD   ( pak, CharInfo->charid );
        ADDWORD    ( pak, clientid );
        ADDWORD    ( pak, GetMaxHP( ) );
        ADDWORD    ( pak, Stats->HP );
        ADDDWORD   ( pak, 0x01000000 );
        ADDDWORD   ( pak, 0x0000000f );
        ADDWORD    ( pak, 0x1388 );
        Party->party->SendToMembers( &pak);
    }

    lastShowTime = clock();
    return true;
}


// HP/MP Regeneration Function
bool CPlayer::Regeneration()
{
    if (Stats->MaxHP==Stats->HP && Stats->MaxMP== Stats->MP)
    {
       lastRegenTime=0;
       return true;
    }

    //LMA REGEN
    bool is_first_regen=false;
    if (lastRegenTime==0)
    {
       is_first_regen=true;
    }

	clock_t etime = clock() - lastRegenTime;
    if( etime >= 8 * CLOCKS_PER_SEC && Stats->HP > 0 )
    {
        unsigned int hpamount = GetHPRegenAmount( );
        unsigned int mpamount = GetMPRegenAmount( );
        Stats->HP += hpamount;
        Stats->MP += mpamount;
        if( Stats->HP > Stats->MaxHP)
            Stats->HP = Stats->MaxHP;
        if( Stats->MP > Stats->MaxMP )
            Stats->MP = Stats->MaxMP;

        if (Stats->MaxHP==Stats->HP && Stats->MaxMP== Stats->MP)
           lastRegenTime=0;
        else
            lastRegenTime = clock();
    }
    return true;
}


// Heal Player when use Food/Pots
bool CPlayer::PlayerHeal()
{
    clock_t transtime = clock() - UsedItem->lastRegTime;
	if( UsedItem->usevalue!=0 && transtime >= 0.3*CLOCKS_PER_SEC )	
    {           
        if( UsedItem->used < UsedItem->usevalue && Stats->HP > 0 )
        {
            int value = UsedItem->userate;                
            if((UsedItem->usevalue - UsedItem->used) < value)
            {
                value = UsedItem->usevalue - UsedItem->used;
            }
            switch( UsedItem->usetype )
            {
                case 16: // HP
                    Stats->HP += value;
                    if(Stats->HP > GetMaxHP())
                        Stats->HP = GetMaxHP();                            
                break;
                case 17: // MP
                    Stats->MP += value;                    
                    if(Stats->MP > GetMaxMP())
                        Stats->MP = GetMaxMP();                        
                break;
            }
            UsedItem->used += value;
            UsedItem->lastRegTime = clock();             
        }
        else
        {
            BEGINPACKET( pak,0x7b7 );
            ADDWORD    ( pak, clientid );
            ADDDWORD   ( pak, GServer->BuildBuffs( this ) );
            switch( UsedItem->usetype )
            {
                case 16: // HP
                    ADDWORD( pak, Stats->HP );                         
                break;
                case 17: // MP
                    ADDWORD( pak, Stats->MP );                     
                break;
            }
            GServer->SendToVisible( &pak, this ); 
            UsedItem->used = 0;
            UsedItem->usevalue = 0;
            UsedItem->userate = 0;
            UsedItem->usetype = 0;
        }
    }
    return true;
}

void CPlayer::ReduceABC( )
{
    unsigned int weapontype = 0;
    weapontype = GServer->EquipList[WEAPON].Index[items[7].itemnum]->type;
    //printf("Reducing ABC item count /n");
    switch(weapontype)
    {
        case 231:
            items[132].count--;
            if(items[132].count<=0)
            {
                ClearBattle( Battle );
                ClearItem( items[132] );
            }
        break;
        case 232:
            items[133].count--;
            if(items[133].count<=0)
            {
                ClearBattle( Battle );
                ClearItem( items[133] );            
            }                   
        break;
        case 233:
            items[134].count--;
            if(items[134].count<=0)
            {
                ClearBattle( Battle );
                ClearItem( items[134] );                                        
            }                   
        break;
        case 271:
            items[132].count--;
            if(items[132].count<=0)
            {
                ClearBattle( Battle );
                ClearItem( items[135] );                    
            }   
        break;
    }     
}

// return party pointer
CParty* CPlayer::GetParty( )
{
    return Party->party;
}

// return intelligence
unsigned int CPlayer::GetInt( )
{
    return Attr->Int;
}

// add item [return item slot [0xffff if couldn't add it]]
unsigned int CPlayer::AddItem( CItem item )
{
    unsigned int newslot=0;
    newslot = GetNewItemSlot( item );
    if(newslot!=0xffff)
    {
        if(items[newslot].count>0)
        {
            unsigned int ntcount = item.count;
            unsigned int utcount = items[newslot].count;            
            if(ntcount+utcount>999)
            {
                item.count = ntcount+utcount - 999;
                items[newslot].count = 999;
                unsigned int otherslot = GetNewItemSlot( item );                                
                if(otherslot!=0xffff)
                {
                    if(items[otherslot].count!=0) items[otherslot].count += item.count;       
                    else items[otherslot] = item;
                    return newslot*1000+otherslot;            
                }
                else
                {
                    items[newslot].count = utcount;
                    return 0xffff; //not inventory space
                }
            }
            else items[newslot].count = ntcount+utcount;
        }
        else items[newslot] = item;
    }
    return newslot;
}

//LMA: Saving slot with a MySQL 4.1+ function
void CPlayer::SaveSlot41( unsigned int slot)
{
   //Update or add a slot (kinky way).
    Log(MSG_INFO,"[Slot41] Tryng to alter slot %i for player %i",slot,CharInfo->charid);
    CalculateSignature(slot);
    int res_mysql=0;
    
	if (items[slot].itemtype > 0) 
    {
        //insert/update
        res_mysql=GServer->DB->QExecuteUpdate("INSERT INTO items (owner,slotnum,itemnum,itemtype,refine,durability,lifespan,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i) ON DUPLICATE KEY UPDATE owner=VALUES(owner),itemnum=VALUES(itemnum),itemtype=VALUES(itemtype),refine=VALUES(refine),durability=VALUES(durability),lifespan=VALUES(lifespan),slotnum=VALUES(slotnum),count=VALUES(count),stats=VALUES(stats),socketed=VALUES(socketed),appraised=VALUES(appraised),gem=VALUES(gem)",
    								CharInfo->charid, slot, items[slot].itemnum, items[slot].itemtype,items[slot].refine, items[slot].durability,
    								items[slot].lifespan, items[slot].count, items[slot].stats, (items[slot].socketed?1:0),
    								(items[slot].appraised?1:0),items[slot].gem );                            	
        Log(MSG_INFO,"[Slot41] Same Update slot %i for player %i (res=%i)...",slot,CharInfo->charid,res_mysql);
		return;
	}

    //delete the slot.
    GServer->DB->QExecute("DELETE FROM items WHERE owner=%i AND slotnum=%i", CharInfo->charid,slot);
    Log(MSG_INFO,"[Slot41] Delete slot %i for player %i",slot,CharInfo->charid);


   return;   
}


//LMA: saving slot in database.
void CPlayer::SaveSlot( unsigned int slot)
{
    //Update or add a slot (kinky way).
    Log(MSG_INFO,"Tryng to alter slot %i for player %i",slot,CharInfo->charid);
    CalculateSignature(slot);
    int res_mysql=0;
    
	if (items[slot].itemtype > 0) 
    {
        //insert if update failed.
        res_mysql=GServer->DB->QExecuteUpdate("UPDATE items  SET itemnum=%i, itemtype=%i, refine=%i, durability=%i, lifespan=%i, count=%i, stats=%i, socketed=%i, appraised=%i ,gem=%i WHERE owner=%i AND slotnum=%i",
							items[slot].itemnum, items[slot].itemtype,items[slot].refine, items[slot].durability,
							items[slot].lifespan, items[slot].count, items[slot].stats, (items[slot].socketed?1:0),
							(items[slot].appraised?1:0),items[slot].gem,CharInfo->charid,slot);
		
		Log(MSG_INFO,"After Update attempt");
        if(res_mysql==0)
        {
           //Slot not in database or nothing has changed.
           Log(MSG_INFO,"Select");
        	MYSQL_RES *result = GServer->DB->QStore("SELECT owner FROM items WHERE owner=%i AND slotnum=%i",CharInfo->charid,slot);
        	if(result==NULL)
            {
               Log(MSG_WARNING,"MySQL Error in SaveSlot");
               return;
            }
           Log(MSG_INFO,"After Select");
            
        	if(mysql_num_rows(result)==0)
        	{
                    Log(MSG_INFO,"insert");
               GServer->DB->QFree( );                                         
               GServer->DB->QExecute("INSERT INTO items (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
    							CharInfo->charid, items[slot].itemnum, items[slot].itemtype,items[slot].refine, items[slot].durability,
    							items[slot].lifespan, slot, items[slot].count, items[slot].stats, (items[slot].socketed?1:0),
    							(items[slot].appraised?1:0),items[slot].gem );
               Log(MSG_INFO,"Insert slot %i for player %i",slot,CharInfo->charid);
               return;                                         
            }
            
            GServer->DB->QFree( );
            Log(MSG_INFO,"Same Update slot %i for player %i...",slot,CharInfo->charid);
       }
       else
       {
           Log(MSG_INFO,"Update slot %i for player %i, nb rows=%i",slot,CharInfo->charid,res_mysql);
       }

		
		return;
	}

    //delete the slot.
    GServer->DB->QExecute("DELETE FROM items WHERE owner=%i AND slotnum=%i", CharInfo->charid,slot);
    Log(MSG_INFO,"Delete slot %i for player %i",slot,CharInfo->charid);


   return;
}

void CPlayer::UpdateInventory( unsigned int slot1, unsigned int slot2 )
{
     Log(MSG_INFO,"In Update Inventory");
    if(slot1==0xffff && slot2==0xffff) return;
    BEGINPACKET( pak, 0x718 );
    //if(slot2!=0xffff && slot2!=0xffff) {ADDBYTE( pak, 2 );}
    if(slot1!=0xffff && slot2!=0xffff) {ADDBYTE( pak, 2 );}
    else {ADDBYTE( pak, 1 );}
    if(slot1!=0xffff)
    {
        ADDBYTE    ( pak, slot1);
        ADDDWORD   ( pak, GServer->BuildItemHead( items[slot1] ) );
        ADDDWORD   ( pak, GServer->BuildItemData( items[slot1] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );
    }    
    if(slot2!=0xffff)
    {
        ADDBYTE    ( pak, slot2 );
        ADDDWORD   ( pak, GServer->BuildItemHead( items[slot2] ) );
        ADDDWORD   ( pak, GServer->BuildItemData( items[slot2] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );
    }
    client->SendPacket( &pak );

    //LMA: MySQL Save slot    
    if(slot1!=0xffff)
         SaveSlot41(slot1);
    if(slot2!=0xffff)
         SaveSlot41(slot2);
}

void CPlayer::reduceItemsLifeSpan( bool attacked)
{
        hits=0;
        for (int i=1; i< 9; i++) //Find check all equip slots for equiped items
        {
            if ( (i!=7 && attacked) || ( i==7 && (!attacked) ) )
            {
             items[i].durabLeft = (items[i].durabLeft<=0 ? items[i].durability*5:items[i].durabLeft-1);
             //Log(MSG_INFO, "DurabLeft: %i", items[i].durabLeft);
             if (items[i].durabLeft < 1)
             if (items[i].itemnum != 0 && items[i].appraised )
             {
                //Log( MSG_INFO, "Reducing item's life span!\nItemId: %i type: %i", items[i].itemnum, items[i].itemtype);
                items[i].lifespan--;
                if (items[i].lifespan < 1)
                {
                   ClearItem( items[i] );
                   BEGINPACKET( pak, 0x7a5);     
                   ADDWORD( pak, clientid );
	               ADDWORD( pak, i);							
	               ADDWORD( pak, 0);
	               ADDWORD( pak, 0);
	               ADDWORD( pak, Stats->Move_Speed );
                   GServer->SendToVisible( &pak,this );    
                }
                   UpdateInventory(i);
             }
            }
        }
}

unsigned CPlayer::getWeaponType()
{
         return GServer->EquipList[WEAPON].Index[items[7].itemnum]->type;
}

//removes bugged or restricted items - By PurpleYouko
bool CPlayer::CheckItems()
{
     CPlayer* thisclient = GServer->GetClientByCharName( CharInfo->charname );
     if(thisclient->items[7].itemtype == 8 && thisclient->items[7].itemnum == 40)
     {
         thisclient->items[7].count = 0;
         ClearItem( thisclient->items[7] );
         GServer->SendPM(thisclient, "Sorry. The Morningstar is a banned item. It has been confiscated");                
         thisclient->SetStats( );
     }
    return true;
 }

//Check for negative zuly by PurpleYouko
bool CPlayer::CheckZulies()
{
    //save character info so that we can look at it later in the database
    CPlayer* thisclient = GServer->GetClientByCharName( CharInfo->charname );
    if((CharInfo->Zulies < 0) || (CharInfo->Storage_Zulies < 0))
    {
        thisclient->savedata();
        //now kick the player out
        BEGINPACKET( pak, 0x702 );
        ADDSTRING( pak, "You have been disconnected from the server for Zuly hacking!" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );
 
        RESETPACKET( pak, 0x707 );
        ADDWORD( pak, 0 );
        thisclient->client->SendPacket( &pak );
 
        thisclient->client->isActive = false;
    }
  return true;
}

bool CPlayer::AddClanPoints(unsigned int count)
{
     if ( Clan== NULL ) return true;
     //load clan info in char server
	BEGINPACKET( pak, 0x7e0 );
	ADDBYTE    ( pak, 0xfb ); //action to update clan informacion (charserver)
	ADDWORD    ( pak, Clan->clanid );
	ADDWORD    ( pak, CharInfo->charid );
	ADDWORD    ( pak, clientid );
	ADDWORD    ( pak, count );
	cryptPacket( (char*)&pak, GServer->cct );
	send( GServer->csock, (char*)&pak, pak.Size, 0 );
	
	printf("added %i clan points\n", count);
}

CClientSocket* CPlayer::getClient()
{
   return client;
}
