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
#ifndef __ROSE_DATATYPES__
#define __ROSE_DATATYPES__

//*upgraded status
//#define NULL 2
#define STRENGTH 10
#define DEXTERY 11
#define INTELIGENCE 12
#define CONCENTRATION 13
#define CHARM 14
#define SENSE 15
#define C_HP 16
#define C_MP 17
#define ATTACK_POWER 18
#define ACCURY 20
#define MAGIC_RESISTENCE 21
#define DODGE 22
#define MOVE_SPEED 23
#define ATTACK_SPEED 24
#define INV_CAPACITY 25
#define CRITICAL 26
#define EXTRA_DAMAGE 36
#define ONE_HAND_WEAP_AP 42
#define TWO_HAND_WEAP_AP 43
#define MOV_SPEED 52
#define DEFENSE 53
#define MAX_HP 54
#define MAX_MP 55
#define HP_REC_AMONT 56
#define MP_REC_RATE 57
#define BAGPACK_CAPACITY 58
#define MP_COST_RED 61
#define SUMMON_GAUGE 62
#define ATK_ACCURACY 99
#define SHIELD_DEFENSE 102
#define ONE_HAND_SWORD_AP 104
#define ONE_HAND_MELEE_AP 105
#define TWO_HAND_SWORD_AP 106
#define TWO_HAND_SPEAR_AP 107
#define TWO_HAND_AXE_AP 108
#define BOW_AP 109
#define GUN_AP 110
#define LAUNCHER_AP 111
#define STAFF_POWER 113
#define WAND_POWER 114
#define KATAR_AP 115
#define DUALSWORD_AP 116
#define BOW_ATK_SPD 123
#define GUN_ATK_SPD 124
#define LAUNCHER_ATK_SPD 125
#define KATAR_ATK_SPD 129 
#define DUALSWORD_ATK_SPD 130

//*status/condition
#define A_STR 10
#define A_DEX 11
#define A_INT 12
#define A_CON 13
#define A_CHA 14
#define A_SEN 15
#define A_HP 16 
#define A_MP 17
#define A_ATTACK 18 
#define A_DEFENSE 19
#define A_ACCUR 20 
#define A_MRESIST 21
#define A_DODGE 22
#define A_DASH 23 
#define A_HASTE 24 
#define A_INVENTORY_CAPACITY 25
#define A_CRITICAL 26
#define A_HP_REC_RATE 27
#define A_MP_REC_RATE 28
#define A_MP_CONSUMATION 29
#define A_Extra_Damage 36
#define A_MAX_HP 38
#define A_MAX_MP 39
#define A_STUN 40
#define A_MUTE 41
#define A_POISON 42


//*spawn values
//BYTE1
#define UNK1 0x01
#define MAX_HP_MP 0x02//revizar
#define POISED 0x04
#define UNK2 0x08
#define MP_UP 0x10//revizar
#define HP_UP 0x20//revizar
#define DASH_UP 0x40
#define DASH_DOWN 0x80
//BYTE2
#define HASTE_UP 0x01
#define HASTE_DOWN 0x02
#define ATTACK_UP 0x04
#define ATTACK_DOWN 0x08
#define DEFENSE_UP 0x10
#define DEFENSE_DOWN 0x20
#define MDEFENSE_UP 0x40
#define MDEFENSE_DOWN 0x80
//BYTE3
#define HITRATE_UP 0x01
#define HITRATE_DOWN 0x02
#define CRITICAL_UP 0x04
#define CRITICAL_DOWN 0x08
#define DODGE_UP 0x10
#define SUMMON 0x20
#define MUTED 0x40
#define SLEEPING 0x80
//BYTE4
#define STUN 0x01
#define INVISIBLE_1 0x02
#define INVISIBLE_2 0x04
#define UNK3 0x08
#define DAMAGE_UP 0x10
#define SUMMON 0x20 
#define UNK4 0x40
#define UNK5 0x80

//Stance
#define ATTACKING 1
#define WALKING 2
#define RUNNING 3
#define DRIVING 4

//Target Type
#define T_MONSTER 1
#define T_PLAYER 2
#define T_SELF 3
#define T_PARTY 4
#define T_OTHER 5

// attack type
#define NORMAL_ATTACK 1
#define SKILL_ATTACK 2
#define SKILL_BUFF 3
#define SKILL_AOE 4
#define SKILL_SELF 5
#define BUFF_SELF 6
#define BUFF_AOE 7
#define AOE_TARGET 8
#define STAY_STILL_ATTACK 15
#define SUMMON_BUFF 16

// other constants
#define MAX_SKILL 60
#define SKILL_DELAY 500 //ms

// weapon type
#define ONE_HAND_SWORD 211
#define ONE_HAND_BUNT_SWORD 212
#define TWO_HAND_SWORD 221
#define SPEAR 222
#define TWO_HAND_AXE 223
#define BOW 231
#define GUN 232
#define LAUNCHER 233
#define STAFF 241
#define WAND 242
#define KATAR 251
#define DOUBLE_SWORD 252
#define DUAL_GUN 253
#define CROSSBOW 271

//equip type
#define MASK 1
#define CAP 2
#define BODY 3
#define GLOVE 4
#define SHOE 5
#define BACK 6
#define JEWEL 7
#define WEAPON 8
#define SUBWEAPON 9
#define CONSUMIBLE 10
#define JEM 11
#define NATURAL 12
#define QUEST 13
#define PAT 14

//time
#define DAY 0
#define MORNING 1
#define EVENING 2
#define NIGHT 3

//LMA: maps
#define NB_CELL_MAX 400

#include "../common/sockets.h"

// Hold party experience when kill a moster
struct CPartyExp
{
    class CParty* thisparty;
    UINT exp;        
    UINT num;
    UINT partymember[50];
    UINT maxlevel;
    bool flag;
};

// Equip information
struct CEquip
{
    UINT id;
    UINT equiptype;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT level;
    UINT material;
    UINT occupation[3];
    UINT condition1[3];
    UINT condition2[3];
    UINT stat1[2];
    UINT stat2[2];
    UINT defense;
    UINT magicresistence;
    UINT attackdistance;
    UINT attackpower;
    UINT attackspeed;
    UINT itemgrade;
    UINT movespeed;
};

// Jem Data
struct CJemData
{
    UINT id;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT material;
    UINT stat1[2];/**/
    UINT stat2[2];/**/
};

// natural data
struct CNaturalData
{
    UINT id;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT pricevalue; 
};

// pat data
struct CPatData
{
    UINT id;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT material;
    UINT partversion;
    UINT maxfuel;
    UINT fuelcons;
    UINT speed;
    UINT attackdistance;
    UINT attackpower;
    UINT attackspeed;  
};

struct CProductData
{
    UINT id;
    UINT item[50];
    UINT amount[50];
};

struct CCSellData
{
    UINT id;
    UINT item[48];
};

struct CUseData
{
    UINT id;
    UINT restriction;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT usecondition[2];
    UINT useeffect[2];
    UINT pricevalue;
};

// List Mob Data
struct CNPCData {  
    UINT id;
    UINT life;
    UINT wspeed;
    UINT rspeed;
    UINT dspeed;
    UINT weapon;
    UINT subweapon;
    UINT level;
    UINT hp;
    UINT atkpower;
    UINT hitrate;
    UINT defense;
    UINT magicdefense;
    UINT dodge;
    float atkspeed;
    UINT AI;
    UINT exp;
  	UINT dropid;    
  	UINT money;
  	UINT item;
  	UINT tab1;
  	UINT tab2;
  	UINT tab3;
  	UINT specialtab;
    float atkdistance;
    UINT aggresive;
    UINT shp;
    UINT dialogid;
    UINT eventid;
};

// Store the damage for monster to give exp
struct MonsterDamage
{
    UINT charid;
    long int damage;
};

struct CDropInfo
{
    BYTE type;
    DWORD item;
    QWORD prob;
};

// For store the drops info
struct CMDrops
{
    UINT id; 
    UINT zuly;
    UINT zulyprob;
    vector<CDropInfo*> Drops;
    UINT level_min;//for map drops
    UINT level_max;//for map drops
    UINT level_boss;//for map drops
    UINT probmax;
};

// For store the Buff info
struct CBuff
{
    unsigned int Buff;//store the buff type
    unsigned int value1;//buff value 1 (direct)
    unsigned int value2;//buff value 2 (%)
    unsigned int Evalue;//Extra value 1 (buffer int +)
    unsigned int Duration; //Skill Duration
    clock_t bufftime;//Buff Time
};

// -----------------------------------------------------------------------------------------
// For store the Buff info
// -----------------------------------------------------------------------------------------
struct CBuffValue
{
    int value1;
    int value2;
    int value3;
};

// -----------------------------------------------------------------------------------------
// For store the Buff info
// -----------------------------------------------------------------------------------------
struct CPSkill
{
    int buff[3];//Buff type
    int value1[3];    
    int value2[3];
};

// -----------------------------------------------------------------------------------------
// For store the clan info
// -----------------------------------------------------------------------------------------
struct SClan
{
    int id;
    int logo;
    int back;
    int grade;
    int cp;  
    char name[50];
    char slogan[100];
    char news[200];
};

// -----------------------------------------------------------------------------------------
// A 2d point, for positions
// -----------------------------------------------------------------------------------------
struct fPoint {
	float x;
	float y;
	float z; // Unused?
};

// -----------------------------------------------------------------------------------------
// GM Respawn Mobs  (50 points max)
// -----------------------------------------------------------------------------------------
struct CRespawnPoints
{
    int d;//flag to duplicate
    int b;//flag to store
    int n;//num points
    int min;//mob min    
    int max;//mob max
    int map;//map
    int mobID;//mob id
    int respawntime;//respawn time
    fPoint points[50];//points
};

// -----------------------------------------------------------------------------------------
// An item that a client owns
// -----------------------------------------------------------------------------------------
struct CItem {
	unsigned		itemnum;
	UINT	itemtype;
	UINT	refine;
	UINT	lifespan;
	UINT	durability;
	bool			socketed;
	bool			appraised;
	int				count;
	UINT 	stats;
	UINT    gem;
	UINT    durabLeft;
	long int sig_head;
	long int sig_data;
	int sig_gem;
	int sp_value;
	int last_sp_value;
};

// -----------------------------------------------------------------------------------------
// Selling / buying items
// -----------------------------------------------------------------------------------------
struct BSItem {
    int slot;
    int count;
    long int price;
    int head;
    int data;
};

// -----------------------------------------------------------------------------------------
// A typical npc
// -----------------------------------------------------------------------------------------
struct CNPC {
	unsigned short clientid;
	fPoint pos;
	float dir;
	unsigned char posMap;
	unsigned npctype;
	CNPCData* thisnpc;
	unsigned dialog;
	long int event;    	
};

// Item data object
struct CItemData
{
	unsigned type;
	unsigned id;
};

// Structure for holding loaded STB data
struct CSTBData {
	unsigned rowcount;
	unsigned fieldcount;
	int** rows;
};

// -----------------------------------------------------------------------------------------
// Skill Data
// -----------------------------------------------------------------------------------------
struct CSkills {        
    UINT id;
    UINT level;
    UINT type;
    UINT range;
    UINT target;
    UINT duration;
    UINT atkpower;
    UINT mp;
    UINT weapon[5];
    UINT rskill[3];
    UINT lskill[3];
    UINT c_class[4];
    UINT clevel;
    UINT success;
    UINT sp;
    UINT buff[3];
    UINT value1[3];
    UINT value2[3];
    UINT nbuffs;
    unsigned short int aoe;
    UINT aoeradius;
    UINT script;
    UINT svalue1;
    UINT gm_aoe;
};

// -----------------------------------------------------------------------------------------
// Sell data object
// -----------------------------------------------------------------------------------------
struct CSellData
{
	unsigned id;
	CItem items[48];
};

// -----------------------------------------------------------------------------------------
// User skill info
// -----------------------------------------------------------------------------------------
struct CSkill
{
	unsigned id;
	unsigned level;
	CSkills* thisskill;
};

// -----------------------------------------------------------------------------------------
// Quest structure
// -----------------------------------------------------------------------------------------
struct CQuest
{
    int id;
    unsigned long int finalid;
    unsigned long int itemid[10];
    UINT QVoffset; //LMA
    UINT QVvalue; //LMA
    BYTE startItems[10];    
    unsigned long int questid;  
    int mobs[10];
    int items[10];    
    int numitems[10];    
    int Itemreward[10];         
    int ItemType[10];
    int CountItem[10];
    int CItem[5];
    int ZulieReward;
    int ExpReward;
    int script;
    UINT value1;
    UINT value2;
    UINT value3;
};

// -----------------------------------------------------------------------------------------
// An item or zuly dropped on the ground
// -----------------------------------------------------------------------------------------
struct CDrop
{
	unsigned short clientid;
	unsigned char posMap;
	fPoint pos;
	char type;
	unsigned amount;
	CItem item;
	time_t droptime;
	unsigned short owner;
    class CParty* thisparty;
};

// -----------------------------------------------------------------------------------------
// pvp zones
// -----------------------------------------------------------------------------------------
struct CPVPZones
{
    int map;
};
// -----------------------------------------------------------------------------------------
// A teleport gate object
// -----------------------------------------------------------------------------------------
struct CTeleGate
{
	unsigned short id;
	fPoint dest;
	unsigned char destMap;
};

// -----------------------------------------------------------------------------------------
// A respawn point object
// -----------------------------------------------------------------------------------------
struct CRespawnPoint
{
	unsigned short id;
	fPoint dest;
	unsigned char destMap;
	unsigned char radius;
	bool masterdest;
};

// -----------------------------------------------------------------------------------------
// A monster spawn zone
// -----------------------------------------------------------------------------------------
struct CSpawnArea
{
	UINT id;
	UINT map;
	UINT min;
	UINT max;
	UINT respawntime;
	UINT montype;
	UINT pcount;
	UINT amon;
	fPoint* points;
	clock_t lastRespawnTime;
	CNPCData* thisnpc;	
	CMDrops* mobdrop;
	CMDrops* mapdrop;
};

struct CUseInfo
{
    int itemnum;
    int itemtype;
    int usescript;
    int usetype;
    int usevalue;
    int use_buff;
};

struct CItemType             
{
    int itemnum;
    int itemtype;
    int probability;
};

struct CItemSlot
{
    UINT newslot[2];
    UINT nslot;
    bool flag;
};

// Buying List (Shop)
struct CBuying
{
    CItem item;
    long int price;
    UINT slot;    
    UINT count;    
};

// Selling List (Shop)
struct CSelling
{
    UINT slot;
    UINT count;
    long int price;
};

// MONSTER <---------------------------
struct MPosition // Monster position
{        
    UINT Map;
    UINT SpawnID;
    fPoint Spos; // Start position
    fPoint Cpos; // Current Position
    fPoint Dpos; // destiny  Position
};

struct MDrop    // monster drops
{
    bool drop; 
    unsigned int firsthit;
    unsigned int firstlevel;
	CMDrops* mobdrop;
	CMDrops* mapdrop;        
};

struct CReward
{
    unsigned int id;
    unsigned int type;
    unsigned int prob;
};
 
struct CChest
{
    unsigned int chestid;
    vector<CReward*> Rewards;
    unsigned int probmax;
};

struct CBValue
{
    UINT NewValue;
    UINT Value;
    UINT Position;
};


// ITEMS
struct CEquipList
{
    CEquip *Index[5000];
    vector<CEquip*> Data;
    CEquip* nullequip;
};

struct CJemList
{
    CJemData *Index[4000];
    vector<CJemData*> Data;
    CJemData* nulljem;
};

struct CNaturalList
{
    CNaturalData *Index[1000];
    vector<CNaturalData*> Data;
    CNaturalData* nullnatural;
};

struct CPatList
{
    CPatData *Index[1000];    
    vector<CPatData*> Data;
    CPatData* nullpat;
};

struct CProductList
{
    CProductData* Index[2000];
    vector<CProductData*> Data;
    CProductData* nullproduct;
};

struct CSellList
{
    CCSellData* Index[1000];
    vector<CCSellData*> Data;
    CCSellData* nullsell;
};

struct CUseList
{
    CUseData* Index[2000];
    vector<CUseData*> Data;
    CUseData* nulluse;
};

struct CMapList
{
    class CMap* Index[300];
    vector<class CMap*> Map;
    class CMap* nullzone;
};

struct CItemStas
{
  UINT stat[2];
  UINT value[2];  
};

struct CFairy
{
    UINT ListIndex;
	clock_t LastTime;
    UINT WaitTime; 
    bool assigned;      
};

//LMA: for map grids:
struct CGridMap
{
    int coords[NB_CELL_MAX]; //10*10 map grid (8*8 for map and a 1 border) (now it can change according to map size).
    int length;       //length of the map
    int width;       //Width of the map
    bool always_on;              //always display or not? (default 0).
    int org_x;                   //point of Origine (never 0,0 and sometimes not the same)...
    int org_y;           
};

struct CListMap
{
    int grid_id;      //Grid ID
    bool always_on;              //always display or not? (default 0).
    int nb_col;                //nb col (according to MINVISUALRANGE)
    int nb_row;                //nb row (according to MINVISUALRANGE)
};

//LMA End

//////////////////////// Geo edit for disassembles 22 oct 07
struct CBreakList
{
    UINT itemnum;
    UINT itemtype;
    UINT product[15];
    UINT amount[15];
    UINT prob[15];
    UINT numToGive;
    UINT total;
};
 


#endif

