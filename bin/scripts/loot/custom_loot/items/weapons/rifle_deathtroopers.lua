rifle_deathtroopers = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/ranged/rifle/rifle_deathtroopers.iff",
	craftingValues = {
		{"mindamage",260,310,0},
		{"maxdamage",320,400,0},
		{"attackspeed",10.7,5.4,0},
		{"woundchance",11,25,0},
		{"hitpoints",750,1500,0},
		{"attackhealthcost",62,31,0},
		{"attackactioncost",53,26,0},
		{"attackmindcost",115,57,0},
		{"roundsused",30,65,0},
		{"zerorangemod",-70,-70,0},
		{"maxrangemod",-10,15,0},
		{"midrange",50,50,0},
		{"midrangemod",10,10,0},
	},
	customizationStringNames = {},
	customizationValues = {},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 500,
	junkDealerTypeNeeded = JUNKWEAPONS+JUNKJAWA,
	junkMinValue = 25,
	junkMaxValue = 45

}
-- this is the bwdl19_rifle
addLootItemTemplate("rifle_deathtroopers", rifle_deathtroopers)
