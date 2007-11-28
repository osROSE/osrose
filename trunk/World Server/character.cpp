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
#include "character.h"
#include "worldserver.h"

//constructor
CCharacter::CCharacter( )
{
    // STATS
    Stats = new STATS;  
    assert(Stats);
    Stats->Level = 0;
    Stats->HP = 0;
    Stats->MP = 0;
    Stats->MaxHP = 0;
    Stats->MaxMP = 0;        
    Stats->Attack_Power = 0;
    Stats->Defense = 0;
    Stats->Critical = 0;
    Stats->Dodge = 0;
    Stats->Accury = 0;
    Stats->Magic_Defense = 0;
    Stats->Move_Speed = 0;
    Stats->Attack_Speed = 0;
    Stats->Attack_Distance = 0;   
    Stats->MaxWeight = 0;
    Stats->MaxSummonGauge = 0;
    Stats->MPReduction = 0;
    Stats->ExtraDamage = 0; //LMA: Devilking / Arnold
    Stats->ExtraDamage_add=0;
    // POSITION  
    Position = new POSITION;    
    assert(Position);    
    Position->Map = 0;
    Position->lastMoveTime = 0;
    Position->saved = 0;
    Position->respawn = 0;
    //BATLE
    Battle = new BATTLE;
    assert(Battle);
    ClearBattle( Battle );  
    Battle->castTime = 0;  
    //STATUS
    Status = new STATUS;
    assert(Status);
        //Stats up
    Status->Attack_up = 0xff;
    Status->Defense_up = 0xff;
    Status->Magic_Defense_up = 0xff;
    Status->Accury_up = 0xff;
    Status->Critical_up = 0xff;
    Status->Dodge_up = 0xff;
    Status->Haste_up = 0xff; 
    Status->Dash_up = 0xff;
    Status->HP_up = 0xff;
    Status->MP_up = 0xff;
    Status->ExtraDamage_up = 0xff;  //LMA: Devilking/Arnold
        //Stats down
    Status->Attack_down = 0xff;
    Status->Defense_down = 0xff;
    Status->Magic_Defense_down = 0xff;
    Status->Accury_down = 0xff;
    Status->Critical_down = 0xff;
    Status->Dodge_down = 0xff;
    Status->Haste_down = 0xff; 
    Status->Dash_down = 0xff;
    Status->HP_down = 0xff;
    Status->MP_down = 0xff;
    Status->ExtraDamage_down = 0xff;  //LMA: Devilking/Arnold
        //Status
    Status->Stun = 0xff;
    //Status->Spoisoned = 0xff;
    //Status->Muted = 0xff;
    //Status->ExtraDamage = 0xff;  //LMA: Devilking/Arnold (was not quoted before)
    Status->CanAttack = true;
    Status->CanRun = true;
    Status->CanMove = true;
    Status->CanCastSkill = true;
    Status->Poison = 0xff;
    Status->Mute = 0xff;
    Status->Flame = 0xff;
        // Stance
    Status->Stance = RUNNING;              
    for(int i=0;i<30;i++)
    {
        MagicStatus[i].Buff = 0;
        MagicStatus[i].Value =0;
        MagicStatus[i].Duration = 0;
        MagicStatus[i].BuffTime = 0;
    }
    CharType = 0;
    clientid = 0xffff;
}

//deconstructor
CCharacter::~CCharacter( )
{
    delete Stats;
    delete Position;
    delete Battle;
    delete Status;
}

// Virtual
bool CCharacter::UpdateValues( )
{
    return true;
}

// update position
void CCharacter::UpdatePosition( bool monster_stay_still )
{
    if(IsOnBattle( ) && Battle->target!=0)
    {
        CCharacter* Target = GetCharTarget( );
        if(Target!=NULL)  
        {
            if(IsMonster())
            {
                float distance = GServer->distance( Position->current, Position->source );
                if(distance>30)
                {
                    OnFar( );
                }
                else
                {
                    if (!monster_stay_still)
                       Position->destiny = Target->Position->current; //MOBS ONLY
                    else
                        Log(MSG_INFO,"This one stays still");
                }
                
            }
            else Position->destiny = Target->Position->current; //ONLY IF NO TARGET ON BATTLE
        }
        else ClearBattle( Battle );
    }
    
    //LMA maps: special case (arrive in Game)
    //and he changed map (GM or scroll or teleporter or boat?)
    //2do: other cases too, all in fact...
    bool is_done=false;
    if (IsPlayer( )&&((last_map==-1&&last_coords==-1)||(last_map!=Position->Map)))
    {
      	//updating player's grid
    	int new_coords=0;
    	int new_map=0;
    	int grid_id=0;

      	//deleting previous presence...
      	if (last_map!=-1&&last_coords!=-1)
      	{
          	grid_id=GServer->allmaps[last_map].grid_id;
            if (grid_id!=-1&&!GServer->allmaps[last_map].always_on&&GServer->gridmaps[grid_id].coords[last_coords]>0)
               GServer->gridmaps[grid_id].coords[last_coords]--;
        }
  
        //New coordinates
    	new_map=Position->Map;
    	grid_id=GServer->allmaps[new_map].grid_id;          	
        new_coords=GServer->GetGridNumber(new_map,(UINT) floor(Position->current.x),(UINT) floor(Position->current.y));
    	last_map=new_map;
    	last_coords=new_coords;
        
    	if (grid_id!=-1||!GServer->allmaps[new_map].always_on)
           GServer->gridmaps[grid_id].coords[new_coords]++;
	
   	    is_done=true;
    }
    
    if(!IsMoving()) return;
	float dx = Position->destiny.x - Position->current.x;
	float dy = Position->destiny.y - Position->current.y;
	float distance = sqrt( (dx*dx) + (dy*dy) );
    float ntime = ( distance / Stats->Move_Speed * GServer->MOVE_SPEED_MODIF ); 
    clock_t etime = clock() - Position->lastMoveTime;
	if (ntime<=etime || distance<1.0 ) 
    {		
        // if (IsPlayer()) printf("Arrived! X: %i, Y: %i\n", (int)Position->current.x, (int)Position->current.y);
		Position->current.x = Position->destiny.x;
		Position->current.y = Position->destiny.y;		
    }
	else 
    {
		Position->current.x += dx*(etime/ntime);
		Position->current.y += dy*(etime/ntime);
	}
	Position->lastMoveTime = clock( );
	
	//LMA: maps (for player)
	if(!IsPlayer()||is_done)
	   return;

	//updating player's grid
	int new_coords=0;
	int new_map=0;
	int grid_id=0;
	
	
	new_map=Position->Map;
	grid_id=GServer->allmaps[new_map].grid_id;	
	new_coords=GServer->GetGridNumber(new_map,(UINT) floor(Position->current.x),(UINT) floor(Position->current.y));
	//changed?
    if (last_map==new_map&&new_coords==last_coords)	
         return;
    
     //Let's update.
	if (grid_id!=-1||!GServer->allmaps[new_map].always_on)
	   GServer->gridmaps[grid_id].coords[new_coords]++;
	
   //deleting player from his previous map
   grid_id=GServer->allmaps[last_map].grid_id;
   if (grid_id!=-1&&!GServer->allmaps[last_map].always_on&&GServer->gridmaps[grid_id].coords[last_coords]>0)
      GServer->gridmaps[grid_id].coords[last_coords]--;
    
    //Log(MSG_INFO,"Now[%i,%i],Was[%i,%i]",new_map,new_coords,last_map,last_coords);	
	last_map=new_map;
	last_coords=new_coords;
}
