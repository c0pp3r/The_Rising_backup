ep3_blackscale_scaleguard = Creature:new {
	customName = "Blackscale Scaleguard",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "trandoshan",
	faction = "",
	level = 105,
	chanceHit = 1.05,
	damageMin = 1150,
	damageMax = 1470,
	baseXp = 7500,
	baseHAM = 50000,
	baseHAMmax = 75000,
	armor = 2,
	resists = {60,60,80,60,55,65,85,45,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/ep3/ep3_blackscale_scaleguard.iff"},
	lootGroups = {
		{
			groups = {
				{group = "clothing_attachments", chance = 2500000},
	    	{group = "pistols", chance = 1000000},
	    	{group = "rifles", chance = 1000000},
	    	{group = "carbines", chance = 1000000},
	    	{group = "bounty_hunter_armor", chance = 250000},
	    	{group = "armor_attachments", chance = 2500000},
	    	{group = "melee_weapons", chance = 1000000},
	    	{group = "kashyyykian_black_mtn_armor", chance = 250000},
	    	{group = "kashyyykian_ceremonial_armor", chance = 250000},
	    	{group = "kashyyykian_hunting_armor", chance = 250000}
			}
		}
	},
	weapons = {"pirate_weapons_heavy"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/slang"
	--attacks = merge(brawlermaster,marksmanmaster,commandomaster,bountyhuntermaster)
}

CreatureTemplates:addCreatureTemplate(ep3_blackscale_scaleguard, "ep3_blackscale_scaleguard")
