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
#include "worldserver.h"
   
// Give Exp
bool CWorldServer::GiveExp( CMonster* thismon, UINT special_lvl, UINT special_exp )
{       
    if( thismon->owner != 0) // Summon 
	{          
        MapList.Index[thismon->Position->Map]->DeleteMonster( thismon );
	    return true;
    }
    // Give Experience Drops and Quest Items
    vector<CPartyExp*> PartyExp;    
    for(UINT i=0;i<thismon->PlayersDamage.size();i++)
    {                     
        MonsterDamage* thisplayer = thismon->PlayersDamage.at(i);              
		CPlayer* thisclient = GetClientByCID( thisplayer->charid, thismon->Position->Map );        		
		if( thisplayer->damage>0 && thisclient!=NULL )
        {                        
    		if( thisclient->Battle->target == thismon->clientid )    
            {     
                ClearBattle( thisclient->Battle )
                thisclient->Position->destiny = thisclient->Position->current; //GIVE EXP FUNCTION
            }                                     
            if( thismon->MonsterDrop->firsthit == thisclient->CharInfo->charid )
            {
                // Give Quest Item
                QUESTS* myquest = thisclient->GetQuestByMob( thismon->montype );
                if( myquest!=0 )
                {
                    BEGINPACKET( pak, 0x731 )
                    ADDWORD    ( pak, thismon->montype );
                    thisclient->client->SendPacket( &pak );                    
                }
            }    
            
            //LMA BEGIN
            //20070621-211100
            //mods for CF...
            unsigned int exp = (unsigned int)ceil((thismon->thisnpc->exp * thisplayer->damage) / (thismon->thisnpc->hp*thismon->thisnpc->level)+special_exp);
            //LMA END
                                  
            /*unsigned int cp = (unsigned int)ceil((thismon->thisnpc->level/6 * thisplayer->damage) / thismon->thisnpc->hp);  
            //ADD Clan points under any condition
            if ((thismon->Position->Map>=11 && thismon->Position->Map<= 13) || thismon->Position->Map== 59) //So far only junon maps
            {
               thisclient->AddClanPoints(GetColorExp( cp));
            }*/
            
            exp = exp * Config.EXP_RATE * 6;            
            if( thisclient->Party->party!=NULL )
            {                                        
                bool pflag = false;
                
                for(int i=0;i<PartyExp.size();i++)
                {
                    CPartyExp* thisparty = PartyExp.at( i );
                    if( thisparty->thisparty == thisclient->Party->party )
                    {
                        thisparty->exp += exp;
                        thisparty->exp += exp * ((thisclient->Party->party->PartyLevel*2) / 100);
                        pflag = true;
                    }
                }
                if( !pflag )
                {
                    CPartyExp* thisparty = new CPartyExp;
                    thisparty->thisparty = thisclient->Party->party;
                    thisparty->exp = exp;
                    thisparty->flag = false;
                    thisparty->exp += exp * ((thisclient->Party->party->PartyLevel*2) / 100);
                    thisparty->num = 1;
                    thisparty->partymember[0] = thisclient->CharInfo->charid;
                    thisparty->maxlevel = thisclient->Stats->Level;
                    for(int p=0;p<thisclient->VisiblePlayers.size();p++)
                    {
                        CPlayer* otherclient = thisclient->VisiblePlayers.at( p );
                        if(otherclient->Party->party==NULL) continue;
                        if( thisclient->Party->party == otherclient->Party->party )
                        {
                            thisparty->partymember[thisparty->num] = otherclient->CharInfo->charid;
                            thisparty->num++;
                            thisparty->maxlevel += otherclient->Stats->Level;
                        }
                    }
                    PartyExp.push_back( thisparty );
                }
                continue;   
            }
            
             //LMA BEGIN
             //20070621-211100
            //mod for CF
            //Adding bonusxp (mileage)
            thisclient->CharInfo->Exp +=  thisclient->bonusxp*GetColorExp( thisclient->Stats->Level, thismon->thisnpc->level + special_lvl, exp );
            //LMA END
            
            BEGINPACKET( pak, 0x79b );
            ADDDWORD   ( pak, thisclient->CharInfo->Exp );
            ADDWORD    ( pak, thisclient->CharInfo->stamina );
            ADDWORD    ( pak, 0 );
            thisclient->client->SendPacket( &pak );                                
        }        
    } 
    for(int p=0;p<PartyExp.size();p++)
    {
        CPartyExp* thisparty = PartyExp.at( p );
        for(int i=0;i<thisparty->num;i++)
        {
            CPlayer* partyclient = GetClientByCID( thisparty->partymember[i], thismon->Position->Map );
            if( partyclient==NULL )
                continue;
            if(partyclient->Party->party==NULL)
                continue;                   
            if(!thisparty->flag)
            {
                partyclient->Party->party->Exp += (thisparty->exp / 6) / 2;
                if( partyclient->Party->party->PartyLevel == 50)
                {
                    partyclient->Party->party->Exp = 0;
                }
                if( partyclient->Party->party->Exp > GetMaxPartyExp( partyclient->Party->party->PartyLevel ) )
                {
                    partyclient->Party->party->PartyLevel++;
                    partyclient->Party->party->Exp -= GetMaxPartyExp( partyclient->Party->party->PartyLevel-1 );
                }                
                BEGINPACKET( pak, 0x7d4 );
                ADDBYTE    ( pak, partyclient->Party->party->PartyLevel );
                ADDDWORD   ( pak, partyclient->Party->party->Exp );
                partyclient->Party->party->SendToMembers( &pak );
                thisparty->flag = true;
            }             
             
            //LMA Begin
            //20070621-211100
            //mods for CF
            unsigned int expoption = partyclient->Party->party->Option%0x80;       
            if( expoption==0 )           
            {
                
                partyclient->CharInfo->Exp +=  GetColorExp( partyclient->Stats->Level, thismon->Stats->Level + special_lvl, (UINT)round(thisparty->exp / thisparty->num) );                
            }
            else
            {
                partyclient->CharInfo->Exp +=  GetColorExp( partyclient->Stats->Level, thismon->Stats->Level + special_lvl, (UINT)round(partyclient->Stats->Level * thisparty->exp / thisparty->maxlevel) );                                                               
            }
            //LMA END
                       		
    		BEGINPACKET( pak, 0x79b );
    		ADDDWORD   ( pak, partyclient->CharInfo->Exp );
    		ADDWORD    ( pak, partyclient->CharInfo->stamina );
    		ADDWORD    ( pak, 0 );
    		partyclient->client->SendPacket( &pak );   
        }            
    }                 
    MapList.Index[thismon->Position->Map]->DeleteMonster( thismon );
    return true;		
}        
