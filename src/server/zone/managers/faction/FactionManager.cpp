/*
 * FactionManager.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: crush
 */

#include "FactionManager.h"
#include "FactionMap.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "templates/manager/TemplateManager.h"
#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/chat/ChatManager.h"
#include "server/zone/packets/player/PlayMusicMessage.h"

FactionManager::FactionManager() {
	setLoggingName("FactionManager");
	setGlobalLogging(false);
	setLogging(false);
}

void FactionManager::loadData() {
	loadLuaConfig();
	loadFactionRanks();
}

void FactionManager::loadFactionRanks() {
	IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/faction/rank.iff");

	if (iffStream == NULL) {
		warning("Faction ranks could not be found.");
		return;
	}

	DataTableIff dtiff;
	dtiff.readObject(iffStream);

	factionRanks.readObject(&dtiff);

	delete iffStream;

	info("loaded " + String::valueOf(factionRanks.getCount()) + " ranks", true);
}

void FactionManager::loadLuaConfig() {
	info("Loading config file.", true);

	FactionMap* fMap = getFactionMap();

	Lua* lua = new Lua();
	lua->init();

	//Load the faction manager lua file.
	lua->runFile("scripts/managers/faction_manager.lua");

	LuaObject luaObject = lua->getGlobalObject("factionList");

	if (luaObject.isValidTable()) {
		for (int i = 1; i <= luaObject.getTableSize(); ++i) {
			LuaObject factionData = luaObject.getObjectAt(i);

			if (factionData.isValidTable()) {
				String factionName = factionData.getStringAt(1);
				bool playerAllowed = factionData.getBooleanAt(2);
				String enemies = factionData.getStringAt(3);
				String allies = factionData.getStringAt(4);
				float adjustFactor = factionData.getFloatAt(5);

				Faction faction(factionName);
				faction.setAdjustFactor(adjustFactor);
				faction.setPlayerAllowed(playerAllowed);
				faction.parseEnemiesFromList(enemies);
				faction.parseAlliesFromList(allies);

				fMap->addFaction(faction);
			}

			factionData.pop();
		}
	}

	luaObject.pop();

	delete lua;
	lua = NULL;
}

FactionMap* FactionManager::getFactionMap() {
	return &factionMap;
}

void FactionManager::awardFactionStanding(CreatureObject* player, const String& factionName, int level) {
	if (player == NULL)
		return;

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (!factionMap.contains(factionName))
		return;

	Faction faction = factionMap.get(factionName);
	SortedVector<String>* enemies = faction.getEnemies();
	SortedVector<String>* allies = faction.getAllies();

	if (!faction.isPlayerAllowed())
		return;

	float gain = level * faction.getAdjustFactor();
	float lose = gain * 2;

	ghost->decreaseFactionStanding(factionName, lose);

	//Lose faction standing to allies of the creature.
	for (int i = 0; i < allies->size(); ++i) {
		String ally = allies->get(i);

		if ((ally == "rebel" || ally == "imperial")) {
			continue;
		}

		if (!factionMap.contains(ally))
			continue;

		Faction allyFaction = factionMap.get(ally);

		if (!allyFaction.isPlayerAllowed())
			continue;

		ghost->decreaseFactionStanding(ally, lose);
	}

	bool gcw = false;
	if (factionName == "rebel" || factionName == "imperial") {
		gcw = true;
	}

	//Gain faction standing to enemies of the creature.
	for (int i = 0; i < enemies->size(); ++i) {
		String enemy = enemies->get(i);

		if ((enemy == "rebel" || enemy == "imperial") && !gcw) {
			continue;
		}

		if (!factionMap.contains(enemy))
			continue;

		Faction enemyFaction = factionMap.get(enemy);

		if (!enemyFaction.isPlayerAllowed())
			continue;

		ghost->increaseFactionStanding(enemy, gain);
	}
}


void FactionManager::awardPvpFactionPoints(TangibleObject* killer, CreatureObject* destructedObject) {
	if (killer->isPlayerCreature() && destructedObject->isPlayerCreature()) {
		CreatureObject* killerCreature = cast<CreatureObject*>(killer);
		ManagedReference<PlayerObject*> ghost = killerCreature->getPlayerObject();

		ManagedReference<PlayerObject*> killedGhost = destructedObject->getPlayerObject();
		ManagedReference<SceneObject*> inventory = killer->getSlottedObject("inventory");
		ManagedReference<LootManager*> lootManager = killer->getZoneServer()->getLootManager();
		ManagedReference<PlayerManager*> playerManager = killerCreature->getZoneServer()->getPlayerManager();

		//Broadcast to Server
		String playerName = destructedObject->getFirstName();
		String killerName = killerCreature->getFirstName();
		StringBuffer zBroadcast;
		zBroadcast << "\\#00e604" << playerName << " \\#e60000 was slain in the GCW by ";

		if (killer->isRebel() && destructedObject->isImperial()) {
			ghost->increaseFactionStanding("rebel", 30);
			killer->playEffect("clienteffect/holoemote_rebel.cef", "head");
			PlayMusicMessage* pmm = new PlayMusicMessage("sound/music_themequest_victory_imperial.snd");
 			killer->sendMessage(pmm);
			lootManager->createLoot(inventory, "holocron_light", 300);//, playerName);
			lootManager->createLoot(inventory, "task_loot_padawan_braid", 300);//, playerName);
			lootManager->createLoot(inventory, "clothing_attachments", 300);//, playerName);
				lootManager->createLoot(inventory, "armor_attachments", 300);//, playerName);
			ghost->decreaseFactionStanding("imperial", 45);
			killedGhost->decreaseFactionStanding("imperial", 45);
			zBroadcast << "\\#00cc99" << killerName;
			ghost->getZoneServer()->getChatManager()->broadcastGalaxy(NULL, zBroadcast.toString());
			if (killerCreature->hasSkill("force_rank_light_novice") && destructedObject->hasSkill("force_rank_dark_novice")) {
				playerManager->awardExperience(killerCreature, "force_rank_xp", 5000);
				playerManager->awardExperience(destructedObject, "force_rank_xp", -7500);
			}
		} else if (killer->isImperial() && destructedObject->isRebel()) {
			ghost->increaseFactionStanding("imperial", 30);
			killer->playEffect("clienteffect/holoemote_imperial.cef", "head");
			PlayMusicMessage* pmm = new PlayMusicMessage("sound/music_themequest_victory_imperial.snd");
 			killer->sendMessage(pmm);
			lootManager->createLoot(inventory, "holocron_dark", 300);//, playerName);
			lootManager->createLoot(inventory, "task_loot_padawan_braid", 300);//, playerName);
			lootManager->createLoot(inventory, "clothing_attachments", 300);//, playerName);
				lootManager->createLoot(inventory, "armor_attachments", 300);//, playerName);
			ghost->decreaseFactionStanding("rebel", 45);
			killedGhost->decreaseFactionStanding("rebel", 45);
			zBroadcast << "\\#00e604" << killerName;
			ghost->getZoneServer()->getChatManager()->broadcastGalaxy(NULL, zBroadcast.toString());
			if (killerCreature->hasSkill("force_rank_dark_novice") && destructedObject->hasSkill("force_rank_light_novice")) {
				playerManager->awardExperience(killerCreature, "force_rank_xp", 5000);
				playerManager->awardExperience(destructedObject, "force_rank_xp", -7500);
			}
		}
	}
}

String FactionManager::getRankName(int idx) {
	if (idx >= factionRanks.getCount())
		return "";

	return factionRanks.getRank(idx).getName();
}

int FactionManager::getRankCost(int rank) {
	if (rank >= factionRanks.getCount())
		return -1;

	return factionRanks.getRank(rank).getCost();
}

int FactionManager::getRankDelegateRatioFrom(int rank) {
	if (rank >= factionRanks.getCount())
		return -1;

	return factionRanks.getRank(rank).getDelegateRatioFrom();
}

int FactionManager::getRankDelegateRatioTo(int rank) {
	if (rank >= factionRanks.getCount())
		return -1;

	return factionRanks.getRank(rank).getDelegateRatioTo();
}

int FactionManager::getFactionPointsCap(int rank) {
	if (rank >= factionRanks.getCount())
		return -1;

	return MAX(1000, getRankCost(rank) * 170);
}

bool FactionManager::isFaction(const String& faction) {
	if (factionMap.contains(faction))
		return true;

	return false;
}

bool FactionManager::isEnemy(const String& faction1, const String& faction2) {

	if (!factionMap.contains(faction1) || !factionMap.contains(faction2))
		return false;

	Faction* faction = factionMap.getFaction(faction1);

	return faction->getEnemies()->contains(faction2);
}

bool FactionManager::isAlly(const String& faction1, const String& faction2) {

	if (!factionMap.contains(faction1) || !factionMap.contains(faction2))
		return false;

	Faction* faction = factionMap.getFaction(faction1);

	return faction->getAllies()->contains(faction2);
}