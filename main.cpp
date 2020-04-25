#include <dllentry.h>
#include <log.h>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include "settings.h"
#include "playerdb.h"
#include <command.h>
#include <Command/Command.h>
#include <Command/CommandRegistry.h>
#include <Command/CommandPermissionLevel.h>
#include <Command/CommandFlag.h>
#include <Command/CommandParameterData.h>
#include <Command/CommandOutput.h>
#include <Command/CommandOrigin.h>
//#include <Actor.h>
#include <Actor/ServerPlayer.h>
#include <Packet/AddActorPacket.h>
#include <Packet/RemoveActorPacket.h>
#include <Packet/BossEventPacket.h>
#include <Packet/MoveActorAbsolutePacket.h>
#include <Packet/UpdateAttributesPacket.h>
#include <Packet/SetActorDataPacket.h>
#include <Actor/ActorUniqueID.h>
#include <Core/DataItem.h>
#include <Core/PacketSender.h>

DEF_LOGGER("EZMod1");
DEFAULT_SETTINGS(settings);

// PS: Reserved for compatibility purposes
// If it is not necessary, keep both functions empty
// Initialization can be done in the PreInit function
// Incorrect use of this function may cause a crash
void dllenter() {}
void dllexit() {}

void PreInit() {
  LOGV("pre init");
  // You can use the event system to receive and process events
  // The following is an example
  Mod::PlayerDatabase::GetInstance().AddListener(SIG("joined"), [](Mod::PlayerEntry const &entry) {
    auto type = entry.player->getEntityTypeId();
    LOGV(EntityTypeToLocString(type, ActorTypeNamespaceRules::IncludeNamespace).c_str());
  });
  //  Mod::PlayerDatabase::GetInstance().AddListener(
  //      SIG("left"), [](Mod::PlayerEntry const &entry) { LOGV("left name: %s, xuid: %d") % entry.name % entry.xuid;
  //      });
}
class HelloWorldCommand : public Command {
public:
  HelloWorldCommand() {}

  static std::vector<std::string> explode(std::string const &s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim);) { result.push_back(std::move(token)); }

    return result;
  }

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    LOGV("Execute helloworld");
    // ActorType type = origin.getEntity()->getEntityTypeId();
    // LOGV(EntityTypeToLocString(type, ActorTypeNamespaceRules::IncludeNamespace).c_str());
    // output.success(EntityTypeToLocString(type, ActorTypeNamespaceRules::IncludeNamespace).c_str());
  }
  static void setup(CommandRegistry *registry) {
    registry->registerCommand(
        "helloworld", "My first command", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);
    registry->registerOverload<HelloWorldCommand>("helloworld");
  }
};
class MakeEntityCommand : public Command {
#pragma region detail
  enum class ExtendedActorType {
    Turtle                 = 4938,
    Chicken                = 4874,
    Phantom                = 68410,
    Bee                    = 378,
    Villager_v2            = 16778099,
    Cat                    = 21323,
    Cow                    = 4875,
    Pig                    = 4876,
    Sheep                  = 4877,
    Wolf                   = 21262,
    Villager               = 16777999,
    Shulker                = 2870,
    Wandering_trader       = 886,
    Mooshroom              = 4880,
    Squid                  = 8977,
    Rabbit                 = 4882,
    Bat                    = 33043,
    Iron_golem             = 788,
    Snow_golem             = 789,
    Ocelot                 = 21270,
    Horse                  = 2118423,
    Llama                  = 4893,
    Polar_bear             = 4892,
    Parrot                 = 21278,
    Dolphin                = 8991,
    Guardian               = 2865,
    Panda                  = 4977,
    Vindicator             = 2873,
    Fox                    = 4985,
    Husk                   = 199471,
    Tropicalfish           = 9071,
    Wither_skeleton        = 1116976,
    Cod                    = 9072,
    Zombie_villager        = 199468,
    Pufferfish             = 9068,
    Witch                  = 2861,
    Salmon                 = 9069,
    Donkey                 = 2118424,
    Mule                   = 2118425,
    Skeleton_horse         = 2186010,
    Zombie_horse           = 2186011,
    Zombie                 = 199456,
    Stray                  = 1116974,
    Drowned                = 199534,
    Creeper                = 2849,
    Skeleton               = 1116962,
    Spider                 = 264995,
    Zombie_pigman          = 68388,
    Slime                  = 2853,
    Enderman               = 2854,
    Silverfish             = 264999,
    Evocation_illager      = 2920,
    Cave_spider            = 265000,
    Vex                    = 2921,
    Ghast                  = 2857,
    Magma_cube             = 2858,
    Blaze                  = 2859,
    Wither                 = 68404,
    Zombie_villager_v2     = 199540,
    Pillager               = 2930,
    Elder_guardian         = 2866,
    Elder_guardian_ghost   = 2936,
    Ender_dragon           = 2869,
    Endermite              = 265015,
    Ravager                = 2875,
    Player                 = 319,
    Minecart               = 524372,
    Hopper_minecart        = 524384,
    Tnt_minecart           = 524385,
    Chest_minecart         = 524386,
    Command_block_minecart = 524388,
    Item                   = 64,
    Tnt                    = 65,
    Falling_block          = 66,
    Moving_block           = 67,
    Xp_bottle              = 4194372,
    Xp_orb                 = 69,
    Eye_of_ender_signal    = 70,
    Ender_crystal          = 71,
    Shulker_bullet         = 4194380,
    Fishing_hook           = 77,
    Dragon_fireball        = 4194383,
    Arrow                  = 12582992,
    Snowball               = 4194385,
    Egg                    = 4194386,
    Painting               = 83,
    Shield                 = 117,
    Thrown_trident         = 12582985,
    Fireball               = 4194389,
    Splash_potion          = 4194390,
    Ender_pearl            = 4194391,
    Leash_knot             = 88,
    Wither_skull           = 4194393,
    Wither_skull_dangerous = 4194395,
    Boat                   = 90,
    Lightning_bolt         = 93,
    Small_fireball         = 4194398,
    Llama_spit             = 4194406,
    Area_effect_cloud      = 95,
    Lingering_potion       = 4194405,
    Evocation_fang         = 4194407,
    Armor_stand            = 317,
    Fireworks_rocket       = 72,
    Agent                  = 312,
    Ice_bomb               = 4194410,
    Balloon                = 107,
    Chalkboard             = 78,
    Npc                    = 307,
    Tripod_camera          = 318
  } eat;
  std::string entityid;
#pragma endregion
public:
  MakeEntityCommand() {}

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    LOGV("Execute makeentity");
    using namespace commands;
    // ActorType type = origin.getEntity()->getEntityTypeId();
    // LOGV(EntityTypeToLocString(type, ActorTypeNamespaceRules::IncludeNamespace).c_str());
    // output.success(EntityTypeToLocString(type, ActorTypeNamespaceRules::IncludeNamespace).c_str());
    auto newEid = origin.getLevel()->getNewUniqueID();
    LOGV("%s") % newEid.value;
    LOGV("%d") % (int)eat;

    auto actor = *Mod::PlayerDatabase::GetInstance().Find((Player *) origin.getEntity());
    switch (eat) {
    case ExtendedActorType::Player: {
      //auto dim = actor.player->getDimensionId().value;
      auto pos = actor.player->getPos();
      //spawn packet
      AddActorPacket pkt;
      SynchedActorData syncedata;
      pkt.uid    = newEid.value;
      pkt.rid    = newEid.value;
      pkt.def_id = ActorDefinitionIdentifier{"minecraft:player"};
      pkt.pos = pos;
      syncedata.append<int64_t>(DataItem::Id::FLAGS, 0);
      syncedata.append<short>(DataItem::Id::AIR, 400);
      syncedata.append<short>(DataItem::Id::MAX_AIR, 400);
      syncedata.append<int64_t>(DataItem::Id::LEAD_HOLDER, -1);
      syncedata.append<std::string>(DataItem::Id::NAMETAG, "FakePlayer");
      syncedata.append<float>(DataItem::Id::SCALE, 1);
      pkt.syncedata = &syncedata;
      actor.player->sendNetworkPacket(pkt);
      //
      output.success("Spawned %d with eid %s", {(int)eat, newEid.value});
    } break;
    default: {
      output.error("Failed to spawn, not implemented", {});
    }
    }
  }
  static void setup(CommandRegistry *registry) {
    using namespace commands;
    registry->registerCommand(
        "makeentity", "Spawn an entity", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);
    addEnum<ExtendedActorType>(registry, "entityActorType", {{"player", ExtendedActorType::Player}});
    registry->registerOverload<MakeEntityCommand>(
        "makeentity",
        mandatory<CommandParameterDataType::ENUM>(&MakeEntityCommand::eat, "entityIdentifier", "entityActorType"));
  }
};
void PostInit() {
  LOGV("post init");
  Mod::CommandSupport::GetInstance().AddListener(SIG("loaded"), &HelloWorldCommand::setup);
  Mod::CommandSupport::GetInstance().AddListener(SIG("loaded"), &MakeEntityCommand::setup);
}

void ServerStart() {
  LOGV("server start");

  /*forEachEntityType([](ActorType type, std::string const &name) {
    std::string str = EntityTypeToLocString(type, ActorTypeNamespaceRules::ExcludeNamespace).c_str();
    //LOGV("rindex %d index dot %d length %d") % str.rfind('.name') % str.find('.') % str.length();
    size_t pos = 0;
    str    = str.substr((pos = str.find('.')) + 1, str.length() - 12);
    str[0] = toupper(str[0]);
    //LOGV("%s = %d") % str % (int) type;
    std::cout << str << " = " << (int) type << std::endl;
    return true;
  });*/
}