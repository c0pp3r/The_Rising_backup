pistol_scatter_light = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/ranged/pistol/pistol_scatter_light.iff",
	craftingValues = {
		{"mindamage",43,87,0},
		{"maxdamage",59,102,0},
		{"attackspeed",5.4,3.9,0},
		{"woundchance",10,24,0},
		{"hitpoints",750,750,0},
		{"attackhealthcost",24,12,0},
		{"attackactioncost",63,35,0},
		{"attackmindcost",24,12,0},
		{"roundsused",30,30,0},
		{"zerorangemod",-45,-25,0},
		{"maxrangemod",-95,-65,0},
		{"midrange",7,7,0},
		{"midrangemod",18,35,0},
	},
	customizationStringNames = {},
	customizationValues = {},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 800,
	junkDealerTypeNeeded = JUNKWEAPONS,
	junkMinValue = 20,
	junkMaxValue = 40

}

addLootItemTemplate("pistol_scatter_light", pistol_scatter_light)
