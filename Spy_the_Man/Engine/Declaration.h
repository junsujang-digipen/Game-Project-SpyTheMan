#pragma once
#include <glm/glm.hpp>
constexpr int ROOMXSIZE{ 960 };
constexpr int ROOMYSIZE{ 540 };
constexpr int WALLTHICKNESS{ 25 };
constexpr int LAYERMAX{ 2 };
constexpr glm::vec4 doorColor{ 202,164,114,255 };
enum class EntityType
{
	Empty,
	Player,
	SideDoor,
	Vent_obj,
	Hide_Obj,
	Room,
	Stair,
	Wall,
	Key,
	Clothes,
	Lazer,
	Basic_NPC,
	Worker_NPC,
	Enemy_NPC,
	Target_NPC,
	UI, Drone, Wave,
	Trap,
	Flashbomb,
	Push,
	Find, Goal, Switch, Count
};

enum class DisguiseList
{
	Player,
	Worker,
	Guard, Conut
};

enum class NPCLevelList {
	Normal,
	Senior,
	Master, Count
};

enum class TaskAIList {
	PointToPoint,
	BasePosition,

};

enum class UILayer : int
{
	DEFAULT = 0, OBJECTUI, MINIMAP, MAPMARK, PHONEUI, SKILLUI, MINIGAME, TALK, PHASE
};

constexpr glm::vec4 HOLOGRAMDEEPCOLOR = { 39, 191, 238, 150 };
constexpr glm::vec4 HOLOGRAMCOLOR = { 39, 191, 238, 50 };

constexpr glm::ivec2 PHONEBUTTONSIZE{ 60,60 };
constexpr glm::vec2 PHONEPOS{ 800,0 };
constexpr glm::vec2 PHONESIZE{ 270,480 };


constexpr glm::vec2 GOALSIZE{270,270};
inline const std::string GOALPATH{ "assets/GameObject/goal.png" };

constexpr glm::vec2 CLOTHESSIZE{ 100,100 };
inline const std::string CLOTHESPATH{ "assets/GameObject/clothes.png" };
constexpr glm::vec2 HIDEOBJSIZE{ 180,270 };
inline const std::string HIDEOBJPATH{ "assets/GameObject/closet.png" };
constexpr glm::vec2 KEYSIZE{ 50,50 };
inline const std::string KEYPATH{ "assets/key.png" };
constexpr glm::vec2 LAZERSIZE{ 100,50 };
constexpr glm::vec2 SIDEDOORSIZE{ 50,300 };
constexpr glm::vec2 STAIRSIZE{ 200 * 0.9,300 * 0.9 };
inline const std::string STAIRPATH{ "assets/GameObject/Stair/upStair.png" };
constexpr glm::vec2 SWITCHSIZE{ 80,80 };
inline const std::string SWITCHPATH{ "assets/GameObject/Switch_On.png" };
constexpr glm::vec2 VENTDOORSIZE{ 100,125 };
inline const std::string VENTDOORPATH{ "assets/ventDoor.png" };
constexpr glm::vec2 WALLSIZE{ 50, 475};

static constexpr int ChapterNum{ 4 };

