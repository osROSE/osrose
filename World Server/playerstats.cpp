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

// Return Dodge
unsigned int CPlayer::GetDodge( )
{
    UINT Dodge = 0;
    Dodge = (UINT)floor(( ((Stats->Level * 0.3) + ((Attr->Dex + Attr->Edex) * 1.9)) + 10 ) * 0.4);
    UINT ClothDodge = 0;
    for(UINT i=1;i<7;i++)
    {
        if( items[i].count != 0 )
            ClothDodge += items[i].durability;
    }    
    Dodge += (UINT)floor(ClothDodge*0.3);
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {    
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_DODGE)
            {
                Dodge += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_DODGE)
            {
                Dodge += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_DODGE)
                    Dodge += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_DODGE)
                    Dodge += GServer->StatsList[items[i].stats].value[1];
            }
        }
    }  
    if(Status->Dodge_up!=0xff)
        Dodge += MagicStatus[Status->Dodge_up].Value;
    if(Status->Dodge_down!=0xff)
        Dodge -= MagicStatus[Status->Dodge_down].Value;     
    if(Dodge<30)
        Dodge=30;        
    return Dodge;
}

// Return Accury
unsigned int CPlayer::GetAccury( )
{
    UINT Accury = 0;
    if(items[7].count==0 )
        Accury = (UINT)floor( (((Attr->Con + Attr->Econ )+10)*0.5) + 15 );
    else
        Accury = (UINT)floor( ((Attr->Con+Attr->Econ+10)*0.8) + ((GServer->EquipList[WEAPON].Index[items[7].itemnum]->quality*0.6) + (items[7].durability*0.8)) );
    UINT extra_refine[10] = { 0, 4, 7, 10, 14, 20, 26, 33, 40, 50};                
    if( items[7].refine>0 )
    {
        UINT refine = (UINT)floor(items[7].refine/16);
        if(refine<10)
        {
            Accury += extra_refine[refine];
        }
    }
    UINT Extra = 0;    
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == ATK_ACCURACY || cskills[i].thisskill->buff[j] ==  A_ACCUR )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        Extra += Accury * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                    {
                        Extra += cskills[i].thisskill->value1[j];
                    }
                }
            }
        }
    }    
    Accury += Extra;  
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_ACCUR)
            {
                Accury += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_ACCUR)
            {
                Accury += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_ACCUR)
                    Accury += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_ACCUR)
                    Accury += GServer->StatsList[items[i].stats].value[1];
            }            
        }
    }       
    if(Status->Accury_up!=0xff)
        Accury += MagicStatus[Status->Dodge_up].Value;
    if(Status->Accury_down!=0xff)
        Accury -= MagicStatus[Status->Accury_down].Value;          
    if(Accury<30)
        Accury=30;
    return Accury;
}

// Return Critical
unsigned int CPlayer::GetCritical( )
{
    UINT Critical = 0;
    Critical = (UINT)floor(((Attr->Con + Attr->Econ + 20)*0.2) + Attr->Sen + Attr->Esen );
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
    	if( cskills[i].thisskill->type == 15 ) //Pasive
        {   // Passive skill fixed by tomiz
            for( UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == A_CRITICAL ) 
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        Critical += Critical * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                        Critical += cskills[i].thisskill->value1[j];
                }
            }
        }
    }    
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0)
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_CRITICAL)
            {
                Critical += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_CRITICAL)
            {
                Critical += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_CRITICAL)
                    Critical += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_CRITICAL)
                    Critical += GServer->StatsList[items[i].stats].value[1];
            }               
        }
    }       
    if(Fairy)Critical = (unsigned int)floor(Critical*1.2);    
    if(Status->Critical_up!=0xff)
        Critical += MagicStatus[Status->Critical_up].Value;
    if(Status->Critical_down!=0xff)
        Critical -= MagicStatus[Status->Critical_down].Value;  
    if(Critical<10)
        Critical=10;     
    return Critical;
}


// Return Magic Defense
unsigned int CPlayer::GetMagicDefense( )
{
    UINT MagicDefense = 0;
    switch( CharInfo->Job )
    {
        case 121:case 122:
        case 221:case 222:
        case 321:case 322:
        case 421:case 422:
            MagicDefense += 20;
        break;
    } 
    MagicDefense += (UINT)floor(((Attr->Int + Attr->Eint+5)*0.6) + ((Stats->Level+15)*0.8));                         
    for(UINT i=1;i<9;i++)
    {        
        if(items[i].itemtype>9)
        {
            Log(MSG_WARNING, "Char %i have equipped invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
            continue;
        }
        if(items[i].itemtype==WEAPON || items[i].count<1 )
            continue;
        MagicDefense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->magicresistence;
    }                        
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_MRESIST)
            {
                MagicDefense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_MRESIST)
            {
                MagicDefense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_MRESIST)
                    MagicDefense += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_MRESIST)
                    MagicDefense += GServer->StatsList[items[i].stats].value[1];
            }                           
        }
    }      
    if(Status->Magic_Defense_up!=0xff)
        MagicDefense += MagicStatus[Status->Magic_Defense_up].Value;
    if(Status->Magic_Defense_down!=0xff)
        MagicDefense -= MagicStatus[Status->Magic_Defense_down].Value;
    return MagicDefense;
}


// Return Attack
unsigned int CPlayer::GetAttackPower( )
{			    
    UINT attack = 0;
    UINT weapontype = 0;   
    UINT weaponatk = 0;      
    switch(CharInfo->Job)
    {
        case 121:case 122:
        case 221:case 222:
        case 321:case 322:
        case 421:case 422:
            attack += 30;
        break;
    }       
	if( items[7].itemnum!=0 && items[7].count > 0 && items[7].durability>0 )
    {
        UINT extra_refine[10] = {0, 7, 14, 21, 31, 41, 51, 65, 80 , 100};                      
        weaponatk = GServer->EquipList[WEAPON].Index[items[7].itemnum]->attackpower;
        weapontype = GServer->EquipList[WEAPON].Index[items[7].itemnum]->type;        
        if( items[7].refine>0 ) 
        {
            UINT refine = (UINT)floor(items[7].refine/16);
            if(refine>0 && refine<10)
            {
                weaponatk += (UINT)floor(extra_refine[refine] * 0.01 * weaponatk);        
            }
        }
    }
    else
    {
        attack += (UINT)floor( Stats->Level*0.2 + (Attr->Str + Attr->Estr)*0.5 + (Attr->Dex + Attr->Edex)*0.3 );
    }
    switch( weapontype )
    {
        case ONE_HAND_SWORD://one hand sword
        {  
            attack += (UINT)floor( (Attr->Str + Attr->Estr)*0.75 + Stats->Level*0.2 + ((Attr->Str + Attr->Estr)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == ONE_HAND_WEAP_AP || 
                            cskills[i].thisskill->buff[j] == ONE_HAND_SWORD_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }    
        }        
        break;
        case ONE_HAND_BUNT_SWORD://one hand blunt sword
            attack += (UINT)floor( (Attr->Str + Attr->Estr)*0.75 + Stats->Level*0.2 + ((Attr->Str + Attr->Estr)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == ONE_HAND_WEAP_AP || 
                            cskills[i].thisskill->buff[j] == ONE_HAND_MELEE_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }           
        break;
        case TWO_HAND_SWORD://two hand sword
        {
            attack += (UINT)floor( (Attr->Str + Attr->Estr)*0.75 + Stats->Level*0.2 + ((Attr->Str + Attr->Estr)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == TWO_HAND_WEAP_AP || 
                            cskills[i].thisskill->buff[j] == TWO_HAND_SWORD_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }        
        } 
        break;
        case SPEAR://spear
        {
            attack += (UINT)floor( (Attr->Str + Attr->Estr)*0.75 + Stats->Level*0.2 + ((Attr->Str + Attr->Estr)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == TWO_HAND_WEAP_AP || 
                            cskills[i].thisskill->buff[j] == TWO_HAND_SPEAR_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }      
        }   
        break;        
        case TWO_HAND_AXE://two hand axe
        {
            attack += (UINT)floor( (Attr->Str + Attr->Estr)*0.75 + Stats->Level*0.2 + ((Attr->Str + Attr->Estr)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == TWO_HAND_WEAP_AP || 
                            cskills[i].thisskill->buff[j] == TWO_HAND_AXE_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }  
        }
        break;
        case BOW://bow
        {    
            float arrows = 0;
            if(items[132].count>0)
                arrows = GServer->NaturalList.Index[items[132].itemnum]->quality*0.5;
            attack += (UINT)floor((((Attr->Str + Attr->Estr) + Stats->Level)*0.1) + ((Attr->Dex + Attr->Edex)*0.04 + (Attr->Sen + Attr->Esen)*0.03 + 29) * weaponatk * 0.03333334 + (Attr->Dex+Attr->Edex)*0.52 + arrows);
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == BOW_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }
        }              
        break;
        case GUN://gun
        case DUAL_GUN://dual guns
        {
            float bullets = 0;
            if(items[133].count>1)
                bullets = GServer->NaturalList.Index[items[133].itemnum]->quality * 0.8;
            attack += (UINT)floor( (Attr->Con + Attr->Econ)*0.47 + bullets + Stats->Level*0.1 + (Attr->Dex + Attr->Edex)*0.3 + ((Attr->Con + Attr->Econ)*0.04 + (Attr->Sen + Attr->Esen)*0.05 + 29) * weaponatk * 0.03333334);            
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == GUN_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            } 
        }             
        break;
        case LAUNCHER://launcher
        {
            float cannons = 0;
            if(items[134].count>0)
            {
                cannons = GServer->NaturalList.Index[items[134].itemnum]->quality;
            }
            attack += (UINT)floor( cannons*0.8 + Stats->Level*0.1 + (Attr->Str + Attr->Estr)*0.32 + (Attr->Con + Attr->Econ)*0.45 + ((Attr->Con + Attr->Econ)*0.04 + (Attr->Sen + Attr->Esen)*0.05 + 29) * weaponatk * 0.03333334 );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == LAUNCHER_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            } 
        }            
        break;
        case STAFF://staff
        {                            
            attack += (UINT)floor( Stats->Level*0.2 + ((Attr->Int + Attr->Eint)+(Attr->Str + Attr->Estr))*0.4 + ((Attr->Int + Attr-> Eint)*0.05 + 29) * weaponatk * 0.03333334 );      
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == STAFF_POWER )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }       
        }       
        break;
        case WAND://wand
        {
            attack += (UINT)floor( Stats->Level*0.2 + (Attr->Int + Attr->Eint)*0.6 + ( ((Attr->Sen + Attr->Esen)*0.1 +26) * 0.03703704 * weaponatk )  );
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == WAND_POWER )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }   
        }           
        break;
        case KATAR://katar
        {
            attack += (UINT)floor( Stats->Level*0.2 + (Attr->Str + Attr->Estr)*0.42 + (Attr->Dex + Attr->Edex)*0.55 + ((Attr->Dex + Attr->Edex)*0.05 + 20) * weaponatk * 0.0476190);               
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == KATAR_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }   
        }       
        break;
        case DOUBLE_SWORD://double sword
        {
            attack += (UINT)floor( Stats->Level*0.2 + (Attr->Str + Attr->Estr)*0.63 + (Attr->Dex + Attr->Edex)*0.45 + ((Attr->Dex + Attr->Edex)*0.05 + 25) * weaponatk * 0.03846154);               
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == DUALSWORD_AP )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                attack += attack * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                attack += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }   
        }           
        break;
        case CROSSBOW://crossbow
        {
            float arrows = 0;
            if(items[132].count>0)
                arrows = GServer->NaturalList.Index[items[132].itemnum]->quality*0.5;
            attack += (UINT)floor((((Attr->Str + Attr->Estr)+Stats->Level)*0.1) + ((Attr->Dex + Attr->Edex)*0.04 + (Attr->Sen +Attr->Esen)*0.03 + 29) * weaponatk * 0.03333334 + (Attr->Dex + Attr->Edex)*0.52 + arrows);
        }
        break;
    }	
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_ATTACK)
            {
                attack += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_ATTACK)
            {
                attack += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].gem!=0)
            {
                if(GServer->JemList.Index[items[i].gem]!=NULL)
                {
                    if( GServer->JemList.Index[items[i].gem]->stat1[0] == ATTACK_POWER)
                    {
                        attack += GServer->JemList.Index[items[i].gem]->stat1[1];
                    }
                    if( GServer->JemList.Index[items[i].gem]->stat2[0] == ATTACK_POWER)
                    {
                        attack += GServer->JemList.Index[items[i].gem]->stat2[1];
                    }                    
                }
            }  
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_ATTACK)
                    attack += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_ATTACK)
                    attack += GServer->StatsList[items[i].stats].value[1];
            }                             
        }
    }     
    if(Fairy)attack = (unsigned int)floor(attack*1.2);
    if(Status->Attack_up!=0xff)
        attack += MagicStatus[Status->Attack_up].Value;
    if(Status->Attack_down!=0xff)
        attack -= MagicStatus[Status->Attack_down].Value;
    if(attack<30)             
        attack = 30;
	return attack;
}

// Return Defense
unsigned int CPlayer::GetDefense( )
{
    UINT defense = 0;
    switch( CharInfo->Job )
    {
        case 121:case 122:
        case 221:case 222:
        case 321:case 322:
        case 421:case 422:
            defense += 25;
        break;
    }        
    defense += (UINT)floor( ((Attr->Str + Attr->Estr)+5) * 0.35 );
    UINT extra_refine[10] = {0, 6, 12, 18, 27, 36, 45, 57, 70 , 85};    
    for(UINT i=1;i<9;i++)
    {
        if(i==7)
            continue;
        if( items[i].count != 0 )
        {         
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }                        
            defense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->defense;
            if(items[i].refine>0)
            {
                UINT refine = (UINT)floor(items[i].refine/16);
                if(refine<10)
                {
                    defense += (UINT)floor(extra_refine[refine] * 0.01 * GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->defense);
                }
            } 
            
        }
    }       
    if(items[8].count>0)//Defense from shield (pasive skill)
    {
        for(UINT i=0;i<MAX_SKILL;i++)
        {
            if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                continue;
            if( cskills[i].thisskill->type == 15 ) //Pasive
            {         
                for( UINT j=0;j<3;j++ )
                {
                    if( cskills[i].thisskill->buff[j] == SHIELD_DEFENSE )
                    {
                        if( cskills[i].thisskill->value2[j] > 0 )
                            defense += defense * cskills[i].thisskill->value2[j] / 100;
                        if( cskills[i].thisskill->value1[j] > 0 )
                            defense += cskills[i].thisskill->value1[j];
                    }
                }
            }
        }              
    }
    
    for(UINT i=0;i<MAX_SKILL;i++)//Defense from pasive skills
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for( UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == DEFENSE || cskills[i].thisskill->buff[j] == A_DEFENSE )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        defense += defense * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                    {
                        defense += cskills[i].thisskill->value1[j];
                    }
                }
            }
        }
    }  
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_DEFENSE)
            {
                defense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_DEFENSE)
            {
                defense += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].gem!=0)
            {
                if(GServer->JemList.Index[items[i].gem]!=NULL)
                {
                    if( GServer->JemList.Index[items[i].gem]->stat1[0] == A_DEFENSE)
                    {
                        defense += GServer->JemList.Index[items[i].gem]->stat1[1];
                    }
                    if( GServer->JemList.Index[items[i].gem]->stat2[0] == A_DEFENSE)
                    {
                        defense += GServer->JemList.Index[items[i].gem]->stat2[1];
                    }                    
                }
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_DEFENSE)
                    defense += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_DEFENSE)
                    defense += GServer->StatsList[items[i].stats].value[1];
            }             
        }
    }      
    if(Fairy) defense = (unsigned int)floor(defense*1.2);
    if(Status->Defense_up!=0xff)
        defense += MagicStatus[Status->Defense_up].Value;
    if(Status->Defense_down!=0xff)
        defense -= MagicStatus[Status->Defense_down].Value;     
    if(defense<30)
        defense=30;             
    return defense;
}

// Return Attack Speed
unsigned int CPlayer::GetAttackSpeed( )
{		  
    UINT aspeed = 0; 
	if( items[7].itemnum!=0 && items[7].count > 0 )
    { 
        UINT wpnspd = 0;
        wpnspd = GServer->EquipList[WEAPON].Index[items[7].itemnum]->attackspeed;
        switch(wpnspd)
        {
            case 12: //normal
                aspeed = 88;
            break;
            case 11: //+1
                aspeed = 93;            
            break;
            case 10: //+2
                aspeed = 100;            
            break;
            case 9: //+3
                aspeed = 107;            
            break;
            case 8: //+4
                aspeed = 115;            
            break;                                                
            case 7: //+5
                aspeed = 125;            
            break;  
            case 6: //+6
                aspeed = 136;            
            break;                          
        }
    }
    else
        aspeed = 115;
    UINT weapontype = 0;
    weapontype = GServer->EquipList[WEAPON].Index[items[7].itemnum]->type;    
    switch(weapontype)
    {
        case 231://bow
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == BOW_ATK_SPD || cskills[i].thisskill->buff[j] == A_HASTE )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                aspeed += aspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                aspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }             
        break;
        case 253:
        case 232://Gun
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == GUN_ATK_SPD || cskills[i].thisskill->buff[j] == A_HASTE )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                aspeed += aspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                aspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }             
        break;        
        case 233://Gun
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == LAUNCHER_ATK_SPD || cskills[i].thisskill->buff[j] == A_HASTE )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                aspeed += aspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                aspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }             
        break;         
        case 251://katar
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == KATAR_ATK_SPD || cskills[i].thisskill->buff[j] == A_HASTE ) 
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                aspeed += aspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                aspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }          
        break;
        case 252://dual weapon
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == DUALSWORD_ATK_SPD || cskills[i].thisskill->buff[j] == A_HASTE )
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                aspeed += aspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                aspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }          
        break;        
    }
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_HASTE)
            {
                aspeed += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_HASTE)
            {
                aspeed += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].gem!=0)
            {
                if(GServer->JemList.Index[items[i].gem]!=NULL)
                {
                    if( GServer->JemList.Index[items[i].gem]->stat1[0] == ATTACK_SPEED)
                    {
                        aspeed += GServer->JemList.Index[items[i].gem]->stat1[1];
                    }
                    if( GServer->JemList.Index[items[i].gem]->stat2[0] == ATTACK_SPEED)
                    {
                        aspeed += GServer->JemList.Index[items[i].gem]->stat2[1];
                    }                    
                }
            }    
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_HASTE)
                    aspeed += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_HASTE)
                    aspeed += GServer->StatsList[items[i].stats].value[1];
            }                         
        }
    }     
    if(Status->Haste_up!=0xff)
        aspeed += MagicStatus[Status->Haste_up].Value;
    if(Status->Haste_down!=0xff)
        aspeed -= MagicStatus[Status->Haste_down].Value;        
    return aspeed;    
}

//LMA: Special Move Speed calcul for cart change (only multiplicator)
unsigned int CPlayer::GetCartSpeed( )
{
       unsigned int mspeed=0;
        UINT porc = 1;
        UINT nb_parts=0;
        float lma_speed;
        
        
        //returns a value only id cart is complete.
        if (items[135].itemnum==0||items[136].itemnum==0||items[137].itemnum==0)
           return 0;
           
        if(items[138].itemnum==0&&items[139].itemnum==0)
           return 0;
                
        for (int k=135;k<138;k++)
          mspeed+=GServer->PatList.Index[items[k].itemnum]->speed;
        
       
    return mspeed;  
}

// Return Movement Speed
unsigned int CPlayer::GetMoveSpeed( )
{
	UINT mspeed = 425;  
	
    if(!Status->CanRun)
        Status->Stance = WALKING;
    switch(Status->Stance)
    {
        case WALKING: //walking
            mspeed = 200;
        break;
        case 1:
        case RUNNING: // running
        {
            if( items[6].count != 0 )//Foot
            {
                UINT tspeed = GServer->EquipList[SHOE].Index[items[6].itemnum]->movespeed;                
        		 mspeed += ( tspeed - 65 ) * 5;
            }
            for(UINT i=0;i<12;i++)//cloth stats [from tomiz]
            {
                if( items[i].count != 0 )
                {
                    if(items[i].itemtype>9)
                    {
                        Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                        continue;
                    }                    
                    if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == MOVE_SPEED)
                    {
                        mspeed += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
                    }
                    if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == MOVE_SPEED)
                    {
                       mspeed += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
                    }
                }
                if(items[i].stats>0 && items[i].stats<500)
                {
                    if(GServer->StatsList[items[i].stats].stat[0] == A_DASH)
                        mspeed += GServer->StatsList[items[i].stats].value[0];
                    if(GServer->StatsList[items[i].stats].stat[1] == A_DASH)
                        mspeed += GServer->StatsList[items[i].stats].value[1];
                }                 
            }
            for(UINT i=0;i<MAX_SKILL;i++)
            {
                if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
                    continue;
                if( cskills[i].thisskill->type == 15 ) //Pasive
                {         
                    for( UINT j=0;j<3;j++ )
                    {
                        if( cskills[i].thisskill->buff[j] == MOV_SPEED || cskills[i].thisskill->buff[j] == A_DASH ) 
                        {
                            if( cskills[i].thisskill->value2[j] > 0 )
                                mspeed += mspeed * cskills[i].thisskill->value2[j] / 100;
                            if( cskills[i].thisskill->value1[j] > 0 )
                            {
                                mspeed += cskills[i].thisskill->value1[j];
                            }
                        }
                    }
                }
            }    
            if(Fairy)  mspeed = (unsigned int)floor(mspeed*1.2);                                      
            if(Status->Dash_up!=0xff)
                mspeed += MagicStatus[Status->Dash_up].Value;
            if(Status->Dash_down!=0xff)
                mspeed -= MagicStatus[Status->Dash_down].Value;                 
        }
        break;
        case DRIVING: //cart
        {
            UINT porc = 1;
            UINT nb_parts=0;
            float lma_speed;
            
            //LMA: new algo for mspeed calculation...
            for (int k=135;k<140;k++)
            {
                 if(items[k].itemnum!=0&&GServer->PatList.Index[items[k].itemnum]->speed>0)
                 {
                    nb_parts++;
                    porc = porc * GServer->PatList.Index[items[k].itemnum]->speed;
                 }
                
            }
            
            if (nb_parts>0)
            {
               lma_speed=floor(porc*1000/(pow(100,nb_parts)));
            }
            
            //add ons (ability)
            switch (items[138].itemnum)
            {
                   case 401:
                        lma_speed+=100;
                        break;
                   case 402:
                        lma_speed+=120;
                        break;
                   case 403:
                        lma_speed+=130;
                        break;
                   case 404:
                        lma_speed+=150;
                        break;
                   case 601:
                        lma_speed+=100;
                        break;
                   default:
                        break;
            }
            
            mspeed= (UINT) lma_speed;
                           
        }
        break;
    }
     
    return mspeed;
}

// Return Max HP
unsigned int CPlayer::GetMaxHP( )
{
	if (Stats->Level<1) Stats->Level=1;
	unsigned int hpmax = (unsigned int)floor(((sqrt(Stats->Level + 20) * (Stats->Level + 5 )) * 3.5) + ((Attr->Str + Attr->Estr) << 1));  	
    switch( CharInfo->Job )
    {
        case 121:case 122:
        case 221:case 222:
        case 321:case 322:
        case 421:case 422:
            hpmax += 300;
        break;
    }
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {   // Passive skill fixed by Kirk
                if( cskills[i].thisskill->buff[j] == A_MAX_HP || cskills[i].thisskill->buff[j] == A_HP || cskills[i].thisskill->buff[j] == MAX_HP)
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        hpmax += hpmax * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                        hpmax += cskills[i].thisskill->value1[j];
                }
            }
        }
    } 
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_MAX_HP || GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_HP )
            {
                hpmax += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_MAX_HP || GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_HP )
            {
                hpmax += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_MAX_HP || GServer->StatsList[items[i].stats].stat[0] == A_HP)
                    hpmax += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_MAX_HP || GServer->StatsList[items[i].stats].stat[0] == A_HP)
                    hpmax += GServer->StatsList[items[i].stats].value[1];
            }                
        }
    }        
    if(Fairy) hpmax = (unsigned int)floor(hpmax*1.2); 
    if(Status->HP_up!=0xff)
        hpmax += MagicStatus[Status->HP_up].Value;
    if(Status->HP_down!=0xff)
        hpmax -= MagicStatus[Status->HP_down].Value;    
                    
    return hpmax;
}
// Return Max MP  [fixed by Tomiz] TODO: add cloth stat
unsigned int CPlayer::GetMaxMP( )
{

    if( Stats->Level < 0 )
        Stats->Level = 1;
    UINT maxmp = 0;
    float Mult = 0;
    int JobValue = (int)CharInfo->Job;
    switch(JobValue)
    {
        case 111:
        case 411:
        case 421:
            Mult = 4;
            JobValue = 3;
        break;
        case 122:
            Mult = 5;
            JobValue = -6;
        break;
        case 211:
            Mult = 6;
            JobValue = 0;
        break;
        case 221:
            Mult = 7;
            JobValue = -7;
        break;
        case 222:
            Mult = 6.5;
            JobValue = -4;
        break;
        case 311:
        case 321:
            Mult = 4;
            JobValue = 4; //LMA
        break;
        case 121:
        case 322:
        case 422:
            Mult = 4.5;
            JobValue = 0;
        break;
        default:
            Mult = 3;
            JobValue = 4;
        break;           
    }
    maxmp = (UINT)floor((Stats->Level + JobValue) * Mult + ((Attr->Int + Attr->Eint) << 2)); 
    
       
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {   // Passive skill fixed by Kirk
                if( cskills[i].thisskill->buff[j] == A_MAX_MP || cskills[i].thisskill->buff[j] == A_MP || cskills[i].thisskill->buff[j] == MAX_MP)
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        maxmp += maxmp * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                        maxmp += cskills[i].thisskill->value1[j];
                }
            }
        }
    }
        
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_MAX_MP || GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_MP )
            {
                maxmp += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_MAX_MP || GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_MP )
            {
                maxmp += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_MAX_MP || GServer->StatsList[items[i].stats].stat[0] == A_MP)
                    maxmp += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_MAX_MP || GServer->StatsList[items[i].stats].stat[0] == A_MP)
                    maxmp += GServer->StatsList[items[i].stats].value[1];
            }               
        }
    }
    

    if(Fairy) maxmp = (unsigned int)floor(maxmp*1.2);     
    if(Status->MP_up!=0xff)
        maxmp += MagicStatus[Status->MP_up].Value;
    if(Status->MP_down!=0xff)
        maxmp -= MagicStatus[Status->MP_down].Value;
        
	return maxmp;
}

// Get HP Regeneration Amount
unsigned int CPlayer::GetHPRegenAmount( )
{
    UINT amount = (UINT)ceil ( Stats->MaxHP * 0.02 );
    /*LMA: handled elsewhere.
    if( Status->Stance == 1 )
        amount *= 4;*/
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {   // Passive skill fixed by Kirk
                if( cskills[i].thisskill->buff[j] == A_HP_REC_RATE || cskills[i].thisskill->buff[j] == HP_REC_AMONT)
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        amount += amount * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                        amount += cskills[i].thisskill->value1[j];
                }
            }
        }
    }                       
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_HP_REC_RATE)
            {
                amount += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_HP_REC_RATE)
            {
                amount += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_HP_REC_RATE )
                    amount += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_HP_REC_RATE )
                    amount += GServer->StatsList[items[i].stats].value[1];
            }                 
        }
    }        

    return amount;
}

// Get MP Regeneration Amount
unsigned int CPlayer::GetMPRegenAmount( )
{
    UINT amount = (UINT)ceil ( Stats->MaxMP * 0.02 );
    /*LMA: Taken elsewhere
    if( Status->Stance == 1 )
        amount *= 4;
    */
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {   // Passive skill fixed by Kirk
                if( cskills[i].thisskill->buff[j] == A_MP_REC_RATE || cskills[i].thisskill->buff[j] == MP_REC_RATE )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        amount += amount * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                        amount += cskills[i].thisskill->value1[j];
                }
            }
        }
    }                             
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0)
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }            
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_MP_REC_RATE)
            {
                amount += amount * GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1] / 100;
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_MP_REC_RATE)
            {
                amount += amount * GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1] / 100;
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_MP_REC_RATE )
                    amount += amount * GServer->StatsList[items[i].stats].value[0] / 100;
                if(GServer->StatsList[items[i].stats].stat[1] == A_MP_REC_RATE )
                    amount += amount * GServer->StatsList[items[i].stats].value[1] / 100;
            }                 
        }
    }                        
    return amount;    
}

// max weight supported
unsigned int CPlayer::GetMaxWeight( )
{
    UINT weight = 0;
    weight = ((Attr->Str + Attr->Estr) + (Attr->Str + Attr->Estr)*2) * 2 + Stats->Level+220 + (Stats->Level+220)*4;
    UINT Extra = 0;    
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == INV_CAPACITY || cskills[i].thisskill->buff[j] == BAGPACK_CAPACITY )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        Extra += weight * cskills[i].thisskill->value2[j] / 100;
                    if( cskills[i].thisskill->value1[j] > 0 )
                    {
                        Extra += cskills[i].thisskill->value1[j];
                    }
                }
            }
        }
    }      
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if( items[i].count != 0 )
        {
            if(items[i].itemtype>9)
            {
                Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
                continue;
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == A_INVENTORY_CAPACITY)
            {
                weight += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == A_INVENTORY_CAPACITY)
            {
                weight += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == A_INVENTORY_CAPACITY )
                    weight += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == A_INVENTORY_CAPACITY )
                    weight += GServer->StatsList[items[i].stats].value[1];
            }                 
        }
    }
    weight += Extra; 
    return weight;
}

// Current weight
unsigned int CPlayer::GetCurrentWeight( )
{
    UINT weight = 0;
    for(UINT i=0;i<MAX_INVENTORY;i++)
    {
        if(items[i].itemtype<10)
        {
            weight += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->weight;                
        }            
        else
        {
            switch(items[i].itemtype)
            {
                case CONSUMIBLE:
                    weight += GServer->UseList.Index[items[i].itemnum]->weight*items[i].count;                        
                break;
                case JEM:
                    weight += GServer->JemList.Index[items[i].itemnum]->weight*items[i].count;                        
                break;
                case NATURAL:
                    weight += GServer->NaturalList.Index[items[i].itemnum]->weight*items[i].count;                        
                break;
                case PAT:
                    weight += GServer->PatList.Index[items[i].itemnum]->weight;                        
                break;
            }
        }
    }
    return weight;
}

// MP Reduction
unsigned int CPlayer::GetMPReduction( )
{
    UINT mpreduction = 0;
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == MP_COST_RED )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        mpreduction += cskills[i].thisskill->value2[j];
                    if( cskills[i].thisskill->value1[j] > 0 )
                        mpreduction += cskills[i].thisskill->value1[j];
                }
            }
        }
    }  
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if(items[i].itemtype>9)
        {
            Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
            continue;
        }
        if(items[i].count>0)
        {
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == MP_COST_RED)
            {
                mpreduction += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == MP_COST_RED)
            {
                mpreduction += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == MP_COST_RED )
                    mpreduction += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == MP_COST_RED )
                    mpreduction += GServer->StatsList[items[i].stats].value[1];
            }           
        }
    } 
    return mpreduction;
}

// Max summon capacity
unsigned int CPlayer::GetMaxSummonGauge( )
{
    UINT gauge = 50;
    for(UINT i=0;i<MAX_SKILL;i++)
    {
        if( cskills[i].id == 0 || cskills[i].thisskill == 0 )
            continue;
        if( cskills[i].thisskill->type == 15 ) //Pasive
        {         
            for(UINT j=0;j<3;j++ )
            {
                if( cskills[i].thisskill->buff[j] == SUMMON_GAUGE )
                {
                    if( cskills[i].thisskill->value2[j] > 0 )
                        gauge += cskills[i].thisskill->value2[j];
                    if( cskills[i].thisskill->value1[j] > 0 )
                        gauge += cskills[i].thisskill->value1[j];
                }
            }
        }
    }  
    for(UINT i=1;i<12;i++)//cloth stats [from tomiz]
    {
        if(items[i].itemtype>9)
        {
            Log(MSG_WARNING, "Char %s have equip invalid item: %i,%i", CharInfo->charname, items[i].itemtype, items[i].itemnum );
            continue;
        }
        if(items[i].count>0)
        {
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0] == SUMMON_GAUGE)
            {
                gauge += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1];
            }
            if( GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0] == SUMMON_GAUGE)
            {
                gauge += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1];
            }
            if(items[i].stats>0 && items[i].stats<500)
            {
                if(GServer->StatsList[items[i].stats].stat[0] == MP_COST_RED )
                    gauge += GServer->StatsList[items[i].stats].value[0];
                if(GServer->StatsList[items[i].stats].stat[1] == MP_COST_RED )
                    gauge += GServer->StatsList[items[i].stats].value[1];
            }     
        }         
    }  
    return gauge;
}

// Extra attributes [str/dex/con/cha/sen/int]
void CPlayer::GetExtraStats( )
{
    Attr->Estr = 0;
    Attr->Edex = 0;
    Attr->Eint = 0;
    Attr->Econ = 0;
    Attr->Esen = 0;
    Attr->Echa = 0;
    for(UINT i=1;i<12;i++)//cloth stats
    {
        if( items[i].count > 0 )
        {
            UINT Stat1 = GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[0];
            UINT Stat2 = GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[0];
            switch(Stat1)
            {
                case A_STR: Attr->Estr += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
                case A_DEX: Attr->Edex += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
                case A_INT: Attr->Eint += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
                case A_CON: Attr->Econ += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
                case A_CHA: Attr->Echa += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
                case A_SEN: Attr->Esen += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat1[1]; break;
            }
            switch(Stat2)
            {
                case A_STR: Attr->Estr += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
                case A_DEX: Attr->Edex += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
                case A_INT: Attr->Eint += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
                case A_CON: Attr->Econ += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
                case A_CHA: Attr->Echa += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
                case A_SEN: Attr->Esen += GServer->EquipList[items[i].itemtype].Index[items[i].itemnum]->stat2[1]; break;
            } 
            if(items[i].gem!=0) // jem stats
            {
                UINT Stat1 = GServer->JemList.Index[items[i].gem]!=NULL?GServer->JemList.Index[items[i].gem]->stat1[0]:0;
                UINT Stat2 = GServer->JemList.Index[items[i].gem]!=NULL?GServer->JemList.Index[items[i].gem]->stat2[0]:0;                
                switch(Stat1)
                {
                    case STRENGTH: Attr->Estr += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                    case DEXTERY: Attr->Edex += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                    case INTELIGENCE: Attr->Eint += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                    case CONCENTRATION: Attr->Econ += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                    case CHARM: Attr->Echa += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                    case SENSE: Attr->Esen += GServer->JemList.Index[items[i].gem]->stat1[1]; break;
                }             
                switch(Stat2)
                {
                    case STRENGTH: Attr->Estr += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                    case DEXTERY: Attr->Edex += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                    case INTELIGENCE: Attr->Eint += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                    case CONCENTRATION: Attr->Econ += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                    case CHARM: Attr->Echa += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                    case SENSE: Attr->Esen += GServer->JemList.Index[items[i].gem]->stat2[1]; break;
                }                    
            } 
            if(items[i].stats!=0 && items[i].stats<500)
            {
                UINT Stat1 = GServer->StatsList[items[i].stats].stat[0];
                UINT Stat2 = GServer->StatsList[items[i].stats].stat[1];
                switch(Stat1)
                {
                    case STRENGTH: Attr->Estr += GServer->StatsList[items[i].stats].value[0]; break;
                    case DEXTERY: Attr->Edex += GServer->StatsList[items[i].stats].value[0]; break;
                    case INTELIGENCE: Attr->Eint += GServer->StatsList[items[i].stats].value[0]; break;
                    case CONCENTRATION: Attr->Econ += GServer->StatsList[items[i].stats].value[0]; break;
                    case CHARM: Attr->Echa += GServer->StatsList[items[i].stats].value[0]; break;
                    case SENSE: Attr->Esen += GServer->StatsList[items[i].stats].value[0]; break;
                }             
                switch(Stat2)
                {
                    case STRENGTH: Attr->Estr += GServer->StatsList[items[i].stats].value[1]; break;
                    case DEXTERY: Attr->Edex += GServer->StatsList[items[i].stats].value[1]; break;
                    case INTELIGENCE: Attr->Eint += GServer->StatsList[items[i].stats].value[1]; break;
                    case CONCENTRATION: Attr->Econ += GServer->StatsList[items[i].stats].value[1]; break;
                    case CHARM: Attr->Echa += GServer->StatsList[items[i].stats].value[1]; break;
                    case SENSE: Attr->Esen += GServer->StatsList[items[i].stats].value[1]; break;
                }                                    
            }     
        }
    }  
}

float CPlayer::GetAttackDistance( )
{
    if(items[7].count==0) return (float)1;
    float AttackDistance = 0;
    AttackDistance = GServer->EquipList[WEAPON].Index[items[7].itemnum]->attackdistance/100;    
    return AttackDistance;
}


// calculate player stats
void CPlayer::SetStats( )
{
    GetExtraStats( );           
    Stats->MaxHP = GetMaxHP( );
    Stats->MaxMP = GetMaxMP( );       
    Stats->Attack_Power = GetAttackPower( );
    Stats->Defense = GetDefense( ); 
    Stats->Critical = GetCritical( );
    Stats->Move_Speed = GetMoveSpeed( );       
    Stats->Magic_Defense = GetMagicDefense( );
    Stats->Accury = GetAccury( );
    Stats->Dodge = GetDodge( );
    Stats->Attack_Speed = GetAttackSpeed( );
    Stats->Attack_Distance = GetAttackDistance( );
    Stats->MaxWeight = GetMaxWeight( );  
    Stats->MaxSummonGauge = GetMaxSummonGauge( );
    Stats->MPReduction = GetMPReduction( );    
}

