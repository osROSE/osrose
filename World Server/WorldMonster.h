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
#ifndef __ROSE_MONSTER__
#define __ROSE_MONSTER__
#include "character.h"
#include "player.h"
#include "worldserver.h"

class CMonster : public CCharacter
{
    public:
        CMonster( fPoint Position, UINT MonsterType, UINT MonsterMap, UINT Owner=0, UINT spawnid=0 );
        ~CMonster( );  
              
        // times
        clock_t SpawnTime;
     	clock_t lastSighCheck;    

        MDrop* MonsterDrop;

    	unsigned int montype;
    	unsigned int owner;	
    	unsigned int hitcount;    //LMA: hit count for some monsters (MC and so on...) Arnold.
    	unsigned int maxhitcount;    //LMA: max hit count for some monsters (MC and so on...) Arnold.
    	bool stay_still;             //LMA: if yes, the monster will never move from the place it had been placed.
    	
    	CParty* thisparty;
    	CNPCData* thisnpc;   
    	
        vector<MonsterDamage*> PlayersDamage;    
        
        // Monster Functions     
        bool SetStats( );
        bool UnspawnMonster( );          		
      	bool IsGhost( );
      	bool IsGhostSeed( );
      	bool IsBonfire( );
      	bool CanMove( );
      	void OnEnemyDie( void* enemy );
      	bool ReadyToMove( );
      	void MoveTo( fPoint nPos, bool randcircle=true );
      	void Move( );
      	CPlayer* GetOwner( );
      	void SpawnMonster( CPlayer* player, CMonster* thismon );      	
      	bool PlayerInRange( );
      	bool PlayerInGrid( );      //LMA: maps
      	CPlayer* GetNearPlayer( UINT mdist = 20 );
        void AddDamage( CCharacter* enemy, long int hitpower);
        CDrop* GetDrop( );
        
        //
        bool IsMonster( );
        bool UpdateValues( );
              	
      	// Stats
        unsigned int GetAttackPower( );
        unsigned int GetDefense( );
        unsigned int GetDodge( );
        unsigned int GetAccury( );
        unsigned int GetMagicDefense( );
        unsigned int GetCritical( );
        unsigned int GetAttackSpeed( );
        unsigned int GetMoveSpeed( );   
        unsigned int GetMaxHP( );    
        float GetAttackDistance( );         	   
        
        // Events
        bool OnBeAttacked( CCharacter* Enemy );
        bool OnEnemyOnSight( CPlayer* Enemy );
        bool OnDie( );
        bool OnEnemyDie( CCharacter* Enemy );
        bool OnSpawn( bool );
        bool OnAlmostDie( );        
        bool OnFar( );
        bool Guardiantree(CMonster* monster,CMap* map);       //LMA: Arnold function for guardian tree
        bool MoonChild(CMonster* monster,CMap* map);          //LMA: For Moon Child
};
#endif
