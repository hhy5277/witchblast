/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>

const std::string APP_NAME =      "Witch Blast";
const std::string APP_VERSION =   "0.3.8";

const std::string CONFIG_FILE =   "config.dat";
const std::string SAVE_FILE =     "game.sav";

const std::string SAVE_VERSION =  "SAVE_0.3.6";

const int NB_LANGUAGES = 5;
const std::string languageString[NB_LANGUAGES] = { "english", "french", "german", "spanish", "russian" };

// Client size
const int SCREEN_WIDTH = 970;
const int SCREEN_HEIGHT = 720;

// Tile set
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;

// Tile map offset
const int OFFSET_X = 5;
const int OFFSET_Y = 5;

const int MAP_WIDTH = 15;
const int MAP_HEIGHT = 9;

const int MENU_MAP_WIDTH = 16;
const int MENU_MAP_HEIGHT = 13;

const int GAME_WIDTH = MAP_WIDTH * TILE_WIDTH;
const int GAME_HEIGHT = MAP_HEIGHT * TILE_HEIGHT;

const int FLOOR_WIDTH = 13;
const int FLOOR_HEIGHT = 7;

const int ITEM_WIDTH = 32;
const int ITEM_HEIGHT = 32;

const int BOLT_WIDTH = 24;
const int BOLT_HEIGHT = 24;
const int BOLT_PRO_LINE = 10;

const int BB_LEFT    = 22;
const int BB_RIGHT   = 22;
const int BB_TOP     = 4;
const int BB_BOTTOM  = 31;

const float FADE_IN_DELAY = 1.0f;
const float FADE_OUT_DELAY = 1.0f;

enum item_images {
  IMAGE_PLAYER_BASE,
  IMAGE_PLAYER_FACES,
  IMAGE_BOLT,
  IMAGE_TILES,
  IMAGE_RAT,
  IMAGE_MINIMAP,
  IMAGE_DOOR,
  IMAGE_ITEMS,
  IMAGE_ITEMS_EQUIP,
  IMAGE_CHEST,
  IMAGE_BAT,
  IMAGE_FLOWER,
  IMAGE_SLIME,
  IMAGE_IMP,
  IMAGE_SPIDER_EGG,
  IMAGE_SPIDER_WEB,
  IMAGE_LITTLE_SPIDER,
  IMAGE_BUBBLE,
  IMAGE_WITCH,
  IMAGE_CAULDRON,
  IMAGE_SNAKE,
  IMAGE_PUMPKIN,

  IMAGE_BUTCHER,
  IMAGE_GIANT_SLIME,
  IMAGE_KING_RAT,
  IMAGE_CYCLOP,
  IMAGE_GIANT_SPIDER,

  IMAGE_BLOOD,
  IMAGE_CORPSES,
  IMAGE_CORPSES_BIG,
  IMAGE_STAR,
  IMAGE_STAR_2,
  IMAGE_INTERFACE,
  IMAGE_HUD_SHOTS,
  IMAGE_EXPLOSION,
  IMAGE_KEYS_QWER,
  IMAGE_KEYS_AZER,
  IMAGE_MESSAGE_ICONS,

  IMAGE_PNJ,
  IMAGE_FAIRY
};

enum sound_resources {
  SOUND_NONE = -1,
  SOUND_BLAST_STANDARD,
  SOUND_BLAST_FLOWER,
  SOUND_DOOR_CLOSING,
  SOUND_DOOR_OPENING,
  SOUND_CHEST_OPENING,
  SOUND_IMPACT,
  SOUND_BONUS,
  SOUND_DRINK,
  SOUND_EAT,
  SOUND_YAWN,
  SOUND_PLAYER_HIT,
  SOUND_PLAYER_DIE,
  SOUND_ENNEMY_DYING,
  SOUND_COIN_PICK_UP,
  SOUND_PAY,
  SOUND_WALL_IMPACT,
  SOUND_BIG_WALL_IMPACT,
  SOUND_KING_RAT_1,
  SOUND_KING_RAT_2,
  SOUND_KING_RAT_DIE,
  SOUND_SLIME_JUMP,
  SOUND_SLIME_IMAPCT,
  SOUND_SLIME_IMAPCT_WEAK,
  SOUND_SLIME_SMASH,
  SOUND_ICE_CHARGE,
  SOUND_ELECTRIC_CHARGE,
  SOUND_SHOT_SELECT,
  SOUND_HEART,
  SOUND_RAT_DYING,
  SOUND_BAT_DYING,
  SOUND_IMP_HURT,
  SOUND_IMP_DYING,
  SOUND_ROCK_IMPACT,
  SOUND_THROW,
  SOUND_CYCLOP_00,
  SOUND_CYCLOP_DIE,
  SOUND_BUTCHER_00,
  SOUND_BUTCHER_01,
  SOUND_BUTCHER_HURT,
  SOUND_BUTCHER_DIE,
  SOUND_VIB,
  SOUND_BOOM_00,
  SOUND_CLANG_00,
  SOUND_BUBBLE_00,
  SOUND_BUBBLE_01,
  SOUND_TRAP,
  SOUND_EGG_SMASH_00,
  SOUND_EGG_SMASH_01,
  SOUND_SPIDER_WALKING,
  SOUND_SPIDER_WEB,
  SOUND_SPIDER_HURT,
  SOUND_SPIDER_DIE,
  SOUND_SPIDER_LITTLE_DIE,
  SOUND_WITCH_00,
  SOUND_WITCH_01,
  SOUND_WITCH_02,
  SOUND_WITCH_DIE_00,
  SOUND_WITCH_DIE_01,
  SOUND_INVOKE,
  SOUND_CAULDRON,
  SOUND_CAULDRON_DIE,
  SOUND_SNAKE_DIE,
  SOUND_PUMPKIN_00,
  SOUND_PUMPKIN_01,
  SOUND_PUMPKIN_DIE,
  SOUND_CRITICAL,
  SOUND_GONG,
  SOUND_TELEPORT,
  SOUND_SPELL_CHARGE,
  SOUND_FIREBALL,
  SOUND_MESSAGE,
  SOUND_EARTHQUAKE,
  SOUND_SPELL_FREEZE,
  SOUND_SPELL_SHIELD
};

enum corpses_ressources{
  FRAME_CORPSE_RAT,
  FRAME_CORPSE_BAT,
  FRAME_CORPSE_FLOWER,
  FRAME_CORPSE_GREEN_RAT,
  FRAME_CORPSE_SLIME,
  FRAME_CORPSE_BLACK_RAT,
  FRAME_CORPSE_IMP_RED,
  FRAME_CORPSE_IMP_BLUE,
  FRAME_CORPSE_SLIME_RED,
  FRAME_CORPSE_SLIME_BLUE,
  FRAME_CORPSE_SPIDER_EGG,
  FRAME_CORPSE_LITTLE_SPIDER,
  FRAME_CORPSE_SPIDER_WEB,
  FRAME_CORPSE_RAT_HELMET,
  FRAME_CORPSE_BLACK_RAT_HELMET,
  FRAME_CORPSE_SLIME_VIOLET,
  FRAME_CORPSE_WITCH,
  FRAME_CORPSE_WITCH_RED,
  FRAME_CORPSE_CAULDRON,
  FRAME_CORPSE_SNAKE,
  FRAME_CORPSE_SNAKE_BLOOD,
  FRAME_CORPSE_PUMPKIN,

  FRAME_CORPSE_KING_RAT,
  FRAME_CORPSE_GIANT_SLIME,
  FRAME_CORPSE_CYCLOP,
  FRAME_CORPSE_BUTCHER,
  FRAME_CORPSE_GIANT_SPIDER
};

// Player game play
const float INITIAL_PLAYER_SPEED = 180.0f;
const int INITIAL_PLAYER_HP = 20;
const float INITIAL_PLAYER_FIRE_DELAY = 0.7f;
const float ACQUIRE_DELAY = 2.8f;
const float UNLOCK_DELAY = 1.0f;
const float INVINCIBLE_DELAY = 1.5f;

const float INITIAL_BOLT_LIFE = 0.45f;
const int INITIAL_BOLT_DAMAGES = 8;
const float INITIAL_BOLT_VELOCITY = 700.0f;
const float INITIAL_BOLT_VISCOSITY = 0.98f;

const float FAIRY_SPEED = 180.0f;
const float FAIRY_FIRE_DELAY = 0.8f;
const float ICE_FAIRY_FIRE_DELAY = 1.3f;
const float TARGET_FAIRY_FIRE_DELAY = 1.0f;
const float FAIRY_BOLT_LIFE = 0.4f;
const int FAIRY_BOLT_DAMAGES = 8;
const int FAIRY_FIRE_DAMAGES = 12;
const float FAIRY_BOLT_VELOCITY = 700.0f;

// chests
enum chest_type_enum {
  ChestBasic,
  ChestExit,
  ChestChallenge,
  ChestFairy,
};
const float CHEST_APPEAR_DELAY = 1.5f;

// Artefact Info
const float ARTEFACT_RECT_WIDTH = 650.0f;
const float ARTEFACT_RECT_HEIGHT = 100.0f;
const float ARTEFACT_POS_Y = 450.0f;
const float ARTEFACT_BORDER = 3.0f;
const float ARTEFACT_ZOOM_TIME = 0.5f;

// shot types
enum enumShotType {
  ShotTypeStandard,
  ShotTypeIce,
  ShotTypeIllusion,
  ShotTypeStone,
  ShotTypeLightning,
  ShotTypeExplodingFire,
  ShotTypeFire,
  ShotTypePoison,
  ShotTypeBomb,
  ShotTypeCold,

  ShotTypeDeterministic
  };

// special shots effects
const int MAX_SHOT_LEVEL = 3;

const float STATUS_FROZEN_DELAY[MAX_SHOT_LEVEL]   // how long the freeze occurs
    = { 4.0f, 5.0f, 6.0f };
const float STATUS_FROZEN_BOLT_DELAY[MAX_SHOT_LEVEL]   // reload time
    = { 3.0f, 2.6f, 2.0f };
const float STATUS_FROZEN_MULT[MAX_SHOT_LEVEL]   // speed multiplier (= 3 times slower)
    = { 0.38f, 0.33f, 0.28f };
const float STONE_DECOIL_DELAY[MAX_SHOT_LEVEL]   // how long the stun occurs
    = { 0.18f, 0.3f, 0.5f };
const float STONE_DECOIL_VELOCITY[MAX_SHOT_LEVEL]   // Decoil velocity
    = { 125.0f, 160.0f, 220.0f };
const float ILLUSION_DAMAGE_DECREASE[MAX_SHOT_LEVEL]   // Illusion damage malus
    = { 0.8f, 0.9f, 1.0f };
const float LIGHTNING_VISCOSITY_INCREASE[MAX_SHOT_LEVEL]   // Air resistance
    = { 0.01, 0.015f, 0.02f };
const float FIRE_DAMAGE_INCREASE[MAX_SHOT_LEVEL]   // Fire damage's bonus
    = { 1.15f, 1.3f, 1.45f };
const int FIRE_EXPLODE_CHANCE[MAX_SHOT_LEVEL]   // Fire exploding chance
    = { 25, 30, 35 };

// entity type
const int ENTITY_PLAYER = 1;
const int ENTITY_FAMILIAR = 2;
const int ENTITY_DOOR = 3;
const int ENTITY_ARTIFACT_DESCRIPTION = 9;
const int ENTITY_BLOOD    = 11;
const int ENTITY_CORPSE   = 12;
const int ENTITY_EFFECT   = 13;
const int ENTITY_FLYING_TEXT = 14;
const int ENTITY_BOLT = 15;
const int ENTITY_ENEMY_BOLT = 16;
const int ENTITY_PNJ = 17;
const int ENTITY_CHEST = 18;
const int ENTITY_ITEM = 19;
const int ENTITY_EXPLOSION = 20;
const int ENTITY_ENEMY = 31;
const int ENTITY_ENEMY_INVOCATED = 32;
const int ENTITY_ENEMY_BOSS = 33;
const int ENTITY_ENEMY_NC = 34;

const int ENTITY_ENEMY_MAX = ENTITY_ENEMY_NC;
const int ENTITY_ENEMY_MAX_COUNT = ENTITY_ENEMY_BOSS;


const float DOOR_OPEN_TIME = 1.0f;
const float DOOR_CLOSE_TIME = 1.0f;

// Rat
const float RAT_SPEED = 185.0f;
const float RAT_SPEED_HELMET = 130.0f;
const int RAT_HP = 16;
const int RAT_HP_HELMET = 24;
const int RAT_DAMAGES = 5;
const int RAT_BB_LEFT = 14;
const int RAT_BB_WIDTH_DIFF = 28;
const int RAT_BB_TOP = 22;
const int RAT_BB_HEIGHT_DIFF = 22;

// Green Rat
const float GREEN_RAT_SPEED = 170.0f;
const int GREEN_RAT_HP = 16;
const int GREEN_RAT_DAMAGES = 5;
const float GREEN_RAT_FADE = 1.0f;

// Black Rat
const float BLACK_RAT_SPEED = 170.0f;
const float BLACK_RAT_SPEED_HELMET = 120.0f;
const int BLACK_RAT_HP = 16;
const int BLACK_RAT_HP_HELMET = 24;
const int BLACK_RAT_DAMAGES = 5;

// Bat
const float BAT_SPEED = 250.0f;
const int BAT_HP = 8;
const int BAT_DAMAGES = 5;

// Evl Flower
const int EVIL_FLOWER_HP = 16;
const int EVIL_FLOWER_MELEE_DAMAGES = 6;
const int EVIL_FLOWER_MISSILE_DAMAGES = 5;
const int EVIL_FLOWER_BB_LEFT = 14;
const int EVIL_FLOWER_BB_WIDTH_DIFF = 28;
const int EVIL_FLOWER_BB_TOP = 22;
const int EVIL_FLOWER_BB_HEIGHT_DIFF = 22;
const float EVIL_FLOWER_FIRE_DELAY = 2.7f;
const float EVIL_FLOWER_FIRE_VELOCITY = 220.0f;

// Snake
const float SNAKE_SPEED = 120.0f;
const int SNAKE_HP = 16;
const int SNAKE_DAMAGE = 5;
const float SNAKE_BLOOD_SPEED = 150.0f;
const int SNAKE_BLOOD_HP = 32;
const int SNAKE_BLOOD_DAMAGE = 8;

// Slime
const int SLIME_HP = 16;
const int SLIME_DAMAGES = 5;
const int SLIME_BB_LEFT = 13;
const int SLIME_BB_WIDTH_DIFF = 26;
const int SLIME_BB_TOP = 38;
const int SLIME_BB_HEIGHT_DIFF = 40;
const float SLIME_FIRE_VELOCITY = 240.0f;

// Imp
const float IMP_SPEED = 180.0f;
const int IMP_HP = 20;
const int IMP_MELEE_DAMAGES = 5;
const int IMP_MISSILE_DAMAGES = 8;
const float IMP_FIRE_VELOCITY = 250.0f;

// Witch
const int WITCH_HP = 80;
const int WITCH_DAMAGE = 5;
const int WITCH_VELOCITY = 130.0f;

// Cauldron
const int CAULDRON_HP = 150;

// Butcher
const int BUTCHER_HP = 150;
const int BUTCHER_DAMAGES = 8;
const int BUTCHER_VELOCITY = 80;

// Giant Slime
const int GIANT_SLIME_HP = 250;
const int GIANT_SLIME_DAMAGES = 8;
const int GIANT_SLIME_MISSILE_DAMAGES = 6;
const float GIANT_SLIME_MISSILE_DELAY = 0.33f;
const float GIANT_SLIME_FIRE_VELOCITY = 200.0f;
const int GIANT_SLIME_BB_LEFT = 26;
const int GIANT_SLIME_BB_WIDTH_DIFF = 52;
const int GIANT_SLIME_BB_TOP = 64;
const int GIANT_SLIME_BB_HEIGHT_DIFF = 12;
const int GIANT_SLIME_SPEED = 85.0f;

// KingRat
const float KING_RAT_SPEED = 200.0f;
const float KING_RAT_RUNNING_SPEED = 600.0f;
const float KING_RAT_BERSERK_SPEED = 250.0f;
const float KING_RAT_RUNNING_RECOIL = 750.0f;
const int KING_RAT_HP = 700;
const int KING_RAT_DAMAGES = 8;

// Cyclops
const float CYCLOP_SPEED[4] = { 120, 130, 140, 150};
const int CYCLOP_NUMBER_ROCKS[4] = { 5, 7, 9, 12};
const float CYCLOP_FIRE_DELAY[4] = { 0.3f, 0.26f, 0.23f, 0.2f};
const int CYCLOP_HP = 700;
const int CYCLOP_DAMAGES = 8;

// Giant Spider
const float GIANT_SPIDER_SPEED[4] = { 230, 235, 240, 245 };
const int GIANT_SPIDER_NUMBER_EGGS[4] = { 12, 14, 16, 19};
const float GIANT_SPIDER_FIRE_DELAY[4] = { 0.8f, 0.7f, 0.6f, 0.5f};
const int GIANT_SPIDER_HP = 800;
const int GIANT_SPIDER_DAMAGE = 8;

// EFFECTS
const float HURTING_DELAY = 0.4f;
const float HEART_BEAT_DELAY = 1.2f;

// messages
const int NB_MSG_LINES = 3;
enum EnumMessages
{
  MsgInfoLevel1,
  MsgInfoLevel2,
  MsgInfoLevel3,
  MsgInfoLevel4,
  MsgInfoLevel5,
  MsgInfoRatsBats,
  MsgInfoSnakes,
  MsgInfoWitches,
  MsgInfoGold,
  MsgInfoFamiliar,
  MsgInfoButcher,
  MsgInfoGiantSlime,
  MsgInfoCyclops,
  MsgInfoWererat,
  MsgInfoGiantSpiderBefore,
  MsgInfoGiantSpiderAfter,

  MsgTutoBasics,
  MsgTutoItems,
  MsgTutoShops,
  MsgTutoBossDoor,
  MsgTutoChallengeDoor,
  MsgTutoHeal,
  MsgTutoShots,
  MsgTutoSpell
};

enum EnumMessagesType
{
  MessageTypeInfo,
  MessageTypeTutorial,
};

struct messageStruct
{
  int icon;
  EnumMessagesType messageType;
  EnumMessages type;
  std::string message[NB_MSG_LINES];
  float timer;
  float timerMax;
};

enum EnumWorldEvents
{
  // info
  EventMeetRatsOrBats,
  EventMeetSnakes,
  EventMeetWitches,
  EventGetCoin,

  // tuto
  EventBeingHurted,
  EventFindShop,
  EventFindBossDoor,
  EventFindChallengeDoor,
  EventGetItem,
  EventGetSpecialShot,
  EventGetFamiliar,
  EventGetSpell,

  NB_EVENTS
};

#endif // CONSTANTS_H_INCLUDED
