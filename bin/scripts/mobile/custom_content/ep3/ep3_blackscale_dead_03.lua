ep3_blackscale_dead_03 = Creature:new {
	customName = "Blackscale Dead",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "trandoshan",
	faction = "",
	level = 30,
	chanceHit = 0.50,
	damageMin = 360,
	damageMax = 400,
	baseXp = 1609,
	baseHAM = 4500,
	baseHAMmax = 5500,
	armor = 1,
	resists = {20,20,20,20,20,20,20,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/ep3/ep3_blackscale_dead_03.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 7000000},
				{group = "skill_buffs", chance = 1000000},
				{group = "loot_kit_parts", chance = 1000000},
				{group = "wearables", chance = 1000000}
			}
		}
	},
	weapons = {"pirate_weapons_heavy"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/slang",
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(ep3_blackscale_dead_03, "ep3_blackscale_dead_03")
