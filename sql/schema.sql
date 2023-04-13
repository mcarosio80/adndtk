CREATE TABLE CAMPAIGN_SETTINGS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(50) NOT NULL
);

CREATE TABLE [EDITION]
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(10) NOT NULL,
	[DESCRIPTION] VARCHAR(50) NOT NULL
);

CREATE TABLE [DIE]
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(4) NOT NULL,
	[DESCRIPTION] VARCHAR(50) NOT NULL
);

CREATE TABLE CHARACTER_CLASS_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(30) NOT NULL,
	[TITLE_LEVEL] INTEGER NULL,
	[TITLE] VARCHAR(30) NULL,
	[HIT_DICE] INTEGER NULL,
	[HP_AFTER_TITLE] INTEGER NULL
);

CREATE TABLE CHARACTER_CLASS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_TYPE_ID] INTEGER NOT NULL,
	[LONG_NAME] VARCHAR(80) NOT NULL,
	[SHORT_NAME] VARCHAR(32) NOT NULL,
	[ACRONYM] VARCHAR(5) NOT NULL,
	FOREIGN KEY(CLASS_TYPE_ID) REFERENCES CHARACTER_CLASS_TYPE(ID)
);

CREATE TABLE RACE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(12) NOT NULL,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[BASE_MOVEMENT_RATE] INTEGER NOT NULL
);

CREATE TABLE SKILL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(12) NOT NULL
);

CREATE TABLE SKILL_REQUISITE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(12) NOT NULL,
	FOREIGN KEY(ID) REFERENCES SKILL(ID)
);

CREATE TABLE SEX
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(12) NOT NULL,
	[ACRONYM] CHAR(1) NOT NULL
);

CREATE TABLE BODY_SLOT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(80) NOT NULL,
	[CAPACITY_LIMIT] INT NULL
);

CREATE TABLE STRENGTH_STATS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ABILITY_SCORE_FROM] INTEGER NOT NULL,
	[EXCP_FROM] INTEGER NULL,
	[ABILITY_SCORE_TO] INTEGER NOT NULL,
	[EXCP_TO] INTEGER NULL,
	[HIT_MODIFIER] INTEGER NOT NULL,
	[DAMAGE_ADJUSTMENT] INTEGER NOT NULL,
	[WEIGHT_ALLOWANCE] FLOAT NOT NULL,
	[MAXIMUM_PRESS] FLOAT NOT NULL,
	[OPEN_DOORS] INTEGER NOT NULL,
	[OPEN_DOORS_SPECIAL] INTEGER NULL,
	[BEND_BARS_LIFT_GATES] INTEGER NOT NULL,
	[NOTE] VARCHAR(50) NULL
);

CREATE TABLE DEXTERITY_STATS
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[REACTION_ADJUSTMENT] INTEGER NOT NULL,
	[MISSILE_ATTACK_ADJUSTMENT] INTEGER NOT NULL,
	[DEFENSIVE_ADJUSTMENT] INTEGER NOT NULL
);

CREATE TABLE CONSTITUTION_STATS
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[HIT_POINT_ADJUSTMENT] INTEGER NOT NULL,
	[HIT_POINT_ADJUSTMENT_WARRIORS] INTEGER NULL,
	[ROLLS_UPGRADE] INTEGER NULL,
	[SYSTEM_SHOCK] INTEGER NOT NULL,
	[RESURRECTION_SURVIVAL] INTEGER NOT NULL,
	[POISON_SAVE] INTEGER NOT NULL,
	[REGENERATION_POINTS] INTEGER NULL,
	[REGENERATION_TURNS] INTEGER NULL
);

CREATE TABLE INTELLIGENCE_STATS
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[NUMBER_OF_LANGUAGES] INTEGER NOT NULL,
	[SPELL_LEVEL] INTEGER NULL,
	[CHANCE_TO_LEARN_SPELL] INTEGER NULL,
	[MAX_NUMBER_OF_SPELLS_PER_LEVEL] INTEGER NULL,
	[ILLUSION_IMMUNITY] INTEGER NULL
);

CREATE TABLE WISDOM_STATS
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[MAGICAL_DEFENCE_ADJUSTMENT] INTEGER NOT NULL,
	[BONUS_SPELL_LEVEL_1] INTEGER NULL,
	[BONUS_SPELL_LEVEL_2] INTEGER NULL,
	[BONUS_SPELL_LEVEL_3] INTEGER NULL,
	[BONUS_SPELL_LEVEL_4] INTEGER NULL,
	[BONUS_SPELL_LEVEL_5] INTEGER NULL,
	[BONUS_SPELL_LEVEL_6] INTEGER NULL,
	[BONUS_SPELL_LEVEL_7] INTEGER NULL,
	[CHANCE_OF_SPELL_FAILURE] INTEGER NOT NULL,
	[SPELL_IMMUNITY] INTEGER NULL
);

--CREATE TABLE SPELL_IMMUNITIES_PER_WISDOM
--(
--	[ID] INTEGER NOT NULL,
--	[SPELL_ID] INTEGER NOT NULL,
--	PRIMARY KEY ([ID], [SPELL_ID])
--);

CREATE TABLE CHARISMA_STATS
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[MAXIMUM_NUMBER_OF_HENCHMEN] INTEGER NOT NULL,
	[LOYALTY_BASE] INTEGER NOT NULL,
	[REACTION_ADJUSTMENT] INTEGER NULL
);

CREATE TABLE SKILL_REQUISITE_VALUES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SKILL_ID] INTEGER NOT NULL,
	[REQUISITE_ID] INTEGER NOT NULL,
	[OBJECT] INTEGER NULL,
	[MIN] INTEGER NOT NULL,
	[MAX] INTEGER NOT NULL,
	FOREIGN KEY(SKILL_ID) REFERENCES SKILL(ID)
);

CREATE TABLE SKILL_MODIFIER
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[RACE_ID] INTEGER NOT NULL,
	[SKILL_ID] INTEGER NOT NULL,
	[VALUE] INTEGER NOT NULL,
	FOREIGN KEY(RACE_ID) REFERENCES RACE(ID),
	FOREIGN KEY(SKILL_ID) REFERENCES SKILL(ID)
);

CREATE TABLE RACIAL_STATS
(
	[RACE_ID] INTEGER NOT NULL PRIMARY KEY,
	[HEIGHT_BASE_MALE] INTEGER NOT NULL,
	[HEIGHT_BASE_FEMALE] INTEGER NOT NULL,
	[HEIGHT_DICE_NUMBER] INTEGER NOT NULL,
	[HEIGHT_DICE_FACES] INTEGER NOT NULL,
	[HEIGHT_DICE_NUMBER_2] INTEGER NULL,
	[HEIGHT_DICE_FACES_2] INTEGER NULL,
	[WEIGHT_BASE_MALE] INTEGER NOT NULL,
	[WEIGHT_BASE_FEMALE] INTEGER NOT NULL,
	[WEIGHT_DICE_NUMBER] INTEGER NOT NULL,
	[WEIGHT_DICE_FACES] INTEGER NOT NULL,
	[WEIGHT_DICE_NUMBER_2] INTEGER NULL,
	[WEIGHT_DICE_FACES_2] INTEGER NULL,
	[AGE_STARTING] INTEGER NOT NULL,
	[AGE_DICE_NUMBER] INTEGER NOT NULL,
	[AGE_DICE_FACES] INTEGER NOT NULL,
	[AGE_MAXIMUM] INTEGER NOT NULL,
	[AGE_MAXIMUM_DICE_NUMBER] INTEGER NOT NULL,
	[AGE_MAXIMUM_DICE_FACES] INTEGER NOT NULL,
	[MIDDLE_AGE] INTEGER NOT NULL,
	[OLD_AGE] INTEGER NOT NULL,
	[VENERABLE_AGE] INTEGER NOT NULL,
	FOREIGN KEY(RACE_ID) REFERENCES RACE(ID)
);

CREATE TABLE AGEING_EFFECTS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(20) NOT NULL,
	[STRENGTH_MODIFIER] INTEGER NOT NULL,
	[DEXTERITY_MODIFIER] INTEGER NOT NULL,
	[CONSTITUTION_MODIFIER] INTEGER NOT NULL,
	[INTELLIGENCE_MODIFIER] INTEGER NOT NULL,
	[WISDOM_MODIFIER] INTEGER NOT NULL,
	[CHARISMA_MODIFIER] INTEGER NOT NULL
);

CREATE TABLE PRIMARY_SKILL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_ID] INTEGER NOT NULL,
	[SKILL_ID] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID),
	FOREIGN KEY(SKILL_ID) REFERENCES SKILL(ID)
);

CREATE TABLE TURNABLE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(20) NOT NULL,
	[SCORE] INTEGER NOT NULL
);

CREATE TABLE TURN_EFFECT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(20) NOT NULL,
	[ACRONYM] VARCHAR(2) NULL
);

CREATE TABLE TURN_UNDEAD
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TURNABLE_CODE] INTEGER NOT NULL,
	[LEVEL] INTEGER NOT NULL,
	[EFFECT] INTEGER NOT NULL,
	[VALUE] INTEGER NULL,
	FOREIGN KEY(TURNABLE_CODE) REFERENCES TURNABLE(ID)
);

CREATE TABLE SAVING_THROW
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(50) NOT NULL,
	[SCORE] INTEGER NOT NULL
);

CREATE TABLE SAVING_THROW_SCORES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_TYPE_ID] INTEGER NOT NULL,
	[LEVEL] INTEGER NOT NULL,
	[SAVING_THROW_ID] INTEGER NOT NULL,
	[SCORE] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_TYPE_ID) REFERENCES CHARACTER_CLASS_TYPE(ID),
	FOREIGN KEY(SAVING_THROW_ID) REFERENCES SAVING_THROW(ID)
);

CREATE TABLE THIEF_ABILITY
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(20) NOT NULL
);

CREATE TABLE THIEF_ABILITY_SCORES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[LEVEL] INTEGER NOT NULL,
	[THIEF_ABILITY_ID] INTEGER NOT NULL,
	[SCORE] INTEGER NOT NULL,
	FOREIGN KEY(THIEF_ABILITY_ID) REFERENCES THIEF_ABILITY(ID)
);

CREATE TABLE THACO
(
	[CLASS_TYPE_ID] INTEGER NOT NULL PRIMARY KEY,
	[SCORE] INTEGER NOT NULL,
	[FACTOR] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_TYPE_ID) REFERENCES CHARACTER_CLASS_TYPE(ID)
);

CREATE TABLE LEVEL_ADVANCEMENT_FACTOR
(
	[CLASS_ID] INTEGER NOT NULL PRIMARY KEY,
	[SCORE] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID)
);

CREATE TABLE LEVEL_ADVANCEMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_ID] INTEGER NOT NULL,
	[LEVEL] INTEGER NOT NULL,
	[SCORE] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID)
);

CREATE TABLE CLASS_AVAILABILITY
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[RACE_ID] INTEGER NOT NULL,
	[CLASS_ID] INTEGER NOT NULL,
	[LEVEL_LIMIT] INTEGER NULL,
	FOREIGN KEY(RACE_ID) REFERENCES RACE(ID),
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID)
);

CREATE TABLE WARRIOR_MELEE_ATTACKS
(
	[LEVEL_FROM] INTEGER NOT NULL,
	[LEVEL_TO] INTEGER NULL,
	[ATTACKS] INTEGER NOT NULL,
	[ROUNDS] INTEGER NOT NULL
);

CREATE TABLE PALADIN_SPELL_PROGRESSION
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[CASTING_LEVEL] INTEGER NULL,
	[SPELL_LEVEL_1] INTEGER NULL,
	[SPELL_LEVEL_2] INTEGER NULL,
	[SPELL_LEVEL_3] INTEGER NULL,
	[SPELL_LEVEL_4] INTEGER NULL
);

CREATE TABLE RANGER_ABILITIES
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[HIDE_IN_SHADOWS] INTEGER NOT NULL,
	[MOVE_SILENTLY] INTEGER NOT NULL,
	[CASTING_LEVEL] INTEGER NULL,
	[SPELL_LEVEL_1] INTEGER NULL,
	[SPELL_LEVEL_2] INTEGER NULL,
	[SPELL_LEVEL_3] INTEGER NULL
);

CREATE TABLE WIZARD_SPELL_PROGRESSION
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[SPELL_LEVEL_1] INTEGER NOT NULL,
	[SPELL_LEVEL_2] INTEGER NULL,
	[SPELL_LEVEL_3] INTEGER NULL,
	[SPELL_LEVEL_4] INTEGER NULL,
	[SPELL_LEVEL_5] INTEGER NULL,
	[SPELL_LEVEL_6] INTEGER NULL,
	[SPELL_LEVEL_7] INTEGER NULL,
	[SPELL_LEVEL_8] INTEGER NULL,
	[SPELL_LEVEL_9] INTEGER NULL
);

CREATE TABLE SCHOOL_OF_MAGIC
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[ALIAS] VARCHAR(50) NULL,
	[CLASS_ID] INTEGER NOT NULL,
	[SKILL_ID] INTEGER NOT NULL,
	[SKILL_VALUE_REQUIRED] INTEGER NOT NULL,
	FOREIGN KEY(SKILL_ID) REFERENCES SKILL(ID),
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID)
);

CREATE TABLE SCHOOL_OF_MAGIC_ACCESS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SCHOOL_ID] INTEGER NOT NULL,
	[CLASS_ID] INTEGER NOT NULL,
	FOREIGN KEY(SCHOOL_ID) REFERENCES SCHOOL_OF_MAGIC(ID),
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(CLASS_ID)
);

CREATE TABLE SCHOOL_OF_MAGIC_PER_RACES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SCHOOL_ID] INTEGER NOT NULL,
	[RACE_ID] INTEGER NOT NULL,
	FOREIGN KEY(SCHOOL_ID) REFERENCES SCHOOL_OF_MAGIC(ID),
	FOREIGN KEY(RACE_ID) REFERENCES RACE(ID)
);

CREATE TABLE SCHOOL_OF_MAGIC_OPPOSITION
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SCHOOL_ID] INTEGER NOT NULL,
	[OPPOSITE_ID] INTEGER NOT NULL,
	FOREIGN KEY(SCHOOL_ID) REFERENCES SCHOOL_OF_MAGIC(ID)
);

CREATE TABLE SPHERE_OF_INFLUENCE_ACCESS_LEVEL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE SPHERE_OF_INFLUENCE_MODE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE SPHERES_OF_INFLUENCE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE WIZARD_SPELL_SCHOOL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SPELL_ID] INT NOT NULL,
	[SCHOOL_ID] INT NOT NULL
);

CREATE TABLE SPHERE_ACCESS_PER_CLASS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_ID] INT NOT NULL,
	[SPHERE_ID] INT NOT NULL,
	[ACCESS_LEVEL] INT NOT NULL,
	[ACCESS_MODE] INT NOT NULL,
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(ID),
	FOREIGN KEY(SPHERE_ID) REFERENCES SPHERES_OF_INFLUENCE(ID),
	FOREIGN KEY(ACCESS_LEVEL) REFERENCES SPHERE_OF_INFLUENCE_ACCESS_LEVEL(ID),
	FOREIGN KEY(ACCESS_MODE) REFERENCES SPHERE_OF_INFLUENCE_MODE(ID)
);

CREATE TABLE PRIEST_SPELL_PROGRESSION
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[SPELL_LEVEL_1] INTEGER NOT NULL,
	[SPELL_LEVEL_2] INTEGER NULL,
	[SPELL_LEVEL_3] INTEGER NULL,
	[SPELL_LEVEL_4] INTEGER NULL,
	[SPELL_LEVEL_5] INTEGER NULL,
	[SPELL_LEVEL_6] INTEGER NULL,
	[SPELL_LEVEL_7] INTEGER NULL
);

CREATE TABLE PRIEST_SPELL_SPHERE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SPELL_ID] INT NOT NULL,
	[SPHERE_ID] INT NOT NULL
);

CREATE TABLE THIEVING_SKILL_RACIAL_ADJUSTMENTS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[THIEVING_SKILL] INTEGER NOT NULL,
	[RACE_ID] INTEGER NOT NULL,
	[MODIFIER] INTEGER NULL,
	FOREIGN KEY(RACE_ID) REFERENCES RACE(ID),
	FOREIGN KEY(THIEVING_SKILL) REFERENCES THIEF_ABILITY(ID)
);

CREATE TABLE THIEVING_SKILL_DEXTERITY_ADJUSTMENTS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DEXTERITY] INTEGER ,
	[THIEVING_SKILL] INTEGER NOT NULL,
	[MODIFIER] INTEGER NULL,
	FOREIGN KEY(THIEVING_SKILL) REFERENCES THIEF_ABILITY(ID)
);

CREATE TABLE THIEVING_SKILL_ARMOUR_ADJUSTMENTS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[THIEVING_SKILL] INTEGER NOT NULL,
	[ARMOUR_ID] INTEGER NULL,
	[MODIFIER] INTEGER NULL,
	FOREIGN KEY(THIEVING_SKILL) REFERENCES THIEF_ABILITY(ID),
	FOREIGN KEY(ARMOUR_ID) REFERENCES EQUIPMENT(ID)
);

CREATE TABLE BACKSTAB_DAMAGE_MULTIPLIER
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[MULTIPLIER] INTEGER NULL
);

CREATE TABLE BARD_SPELL_PROGRESSION
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[SPELL_LEVEL_1] INTEGER NULL,
	[SPELL_LEVEL_2] INTEGER NULL,
	[SPELL_LEVEL_3] INTEGER NULL,
	[SPELL_LEVEL_4] INTEGER NULL,
	[SPELL_LEVEL_5] INTEGER NULL,
	[SPELL_LEVEL_6] INTEGER NULL
);

CREATE TABLE BARD_ABILITIES
(
	[ABILITY_ID] INTEGER NOT NULL PRIMARY KEY,
	[SCORE] INTEGER NOT NULL,
	FOREIGN KEY(ABILITY_ID) REFERENCES THIEF_ABILITY(ID)
);

CREATE TABLE PROFICIENCY_SLOTS
(
	[CLASS_TYPE_ID] INTEGER NOT NULL PRIMARY KEY,
	[WEAPON_INITIAL_SCORE] INTEGER NOT NULL,
	[WEAPON_NUM_LEVELS] INTEGER NOT NULL,
	[PENALTY] INTEGER NOT NULL,
	[NON_WEAPON_INITIAL_SCORE] INTEGER NOT NULL,
	[NON_WEAPON_NUM_LEVELS] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_TYPE_ID) REFERENCES CHARACTER_CLASS_TYPE(ID)
);

CREATE TABLE SPECIALIST_ATTACKS_PER_ROUND
(
	[LEVEL] INTEGER NOT NULL PRIMARY KEY,
	[MELEE_WEAPON_ATTACKS] INTEGER NOT NULL,
	[MELEE_WEAPON_ROUND] INTEGER NOT NULL,
	[LIGHT_X_BOW_ATTACKS] INTEGER NOT NULL,
	[LIGHT_X_BOW_ROUND] INTEGER NOT NULL,
	[HEAVY_X_BOW_ATTACKS] INTEGER NOT NULL,
	[HEAVY_X_BOW_ROUND] INTEGER NOT NULL,
	[THROWN_DAGGER_ATTACKS] INTEGER NOT NULL,
	[THROWN_DAGGER_ROUND] INTEGER NOT NULL,
	[THROWN_DART_ATTACKS] INTEGER NOT NULL,
	[THROWN_DART_ROUND] INTEGER NOT NULL,
	[OTHER_MISSILE_ATTACKS] INTEGER NOT NULL,
	[OTHER_MISSILE_ROUND] INTEGER NOT NULL
);

CREATE TABLE ELEMENT_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE ELEMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TYPE] INT NOT NULL,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE COIN
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE COIN_EXCHANGE_VALUES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[COIN_FROM] INTEGER NOT NULL,
	[COIN_TO] INTEGER NOT NULL,
	[EXCHANGE_VALUE] FLOAT NOT NULL,
	FOREIGN KEY(COIN_FROM) REFERENCES COIN(ID),
	FOREIGN KEY(COIN_TO) REFERENCES COIN(ID)
);

CREATE TABLE STARTING_MONEY
(
	[CLASS_TYPE_ID] INTEGER NOT NULL PRIMARY KEY,
	[DIE_NUMBER] INTEGER NOT NULL,
	[DIE_FACES] INTEGER NOT NULL,
	[DIE_BASE] INTEGER NOT NULL,
	[MULTIPLIER] INTEGER NOT NULL,
	FOREIGN KEY(CLASS_TYPE_ID) REFERENCES CHARACTER_CLASS_TYPE(ID)
);

CREATE TABLE HAND_SLOT_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE ITEM_HAND_REQUIRED
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE ITEM_CLASS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE EQUIPMENT_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE EQUIPMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[TYPE] INTEGER NOT NULL,
	[COST_COIN] INTEGER NOT NULL,
	[COST_MIN] FLOAT NOT NULL,
	[COST_MAX] FLOAT NULL,
	[WEIGHT] FLOAT NULL,
	[BODY_SLOT] INT NULL,
	FOREIGN KEY([TYPE]) REFERENCES EQUIPMENT_TYPE(ID)
);

CREATE TABLE WEAPON_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TYPE] CHAR(1) NOT NULL,
	[DESCRIPTION] VARCHAR(20) NOT NULL
);

CREATE TABLE WEAPON_SIZE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TYPE] CHAR(1) NOT NULL,
	[DESCRIPTION] VARCHAR(20) NOT NULL
);

CREATE TABLE TARGET_SIZE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TYPE] CHAR(1) NOT NULL,
	[DESCRIPTION] VARCHAR(20) NOT NULL
);

CREATE TABLE CLOTHING
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[EQUIPMENT_ID] INTEGER NOT NULL,
	[BODY_SLOT] INTEGER NOT NULL,
	FOREIGN KEY(EQUIPMENT_ID) REFERENCES EQUIPMENT(ID),
	FOREIGN KEY(BODY_SLOT) REFERENCES BODY_SLOT(ID)
);

CREATE TABLE ATTACK_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE ATTACK_RESULT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] VARCHAR(2) NOT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE WEAPON_GROUP
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE WEAPON
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[EQUIPMENT_ID] INTEGER NOT NULL,
	[WEAPON_SIZE] INTEGER NOT NULL,
	[WEAPON_TYPE] INTEGER NOT NULL,
	[SPEED_FACTOR] INTEGER NULL,
	[TARGET_SIZE] INTEGER NOT NULL,
	[DAMAGE_DICE_NUMBER] INTEGER NULL,
	[DAMAGE_DIE_FACES] INTEGER NULL,
	[DAMAGE_MODIFIER] INTEGER NULL,
	[ATTACK_TYPE] INTEGER NULL,
	[WEAPON_GROUP] INTEGER NULL,
	FOREIGN KEY(EQUIPMENT_ID) REFERENCES EQUIPMENT(ID),
	FOREIGN KEY(WEAPON_SIZE) REFERENCES WEAPON_SIZE(ID),
	FOREIGN KEY(WEAPON_TYPE) REFERENCES WEAPON_TYPE(ID),
	FOREIGN KEY(TARGET_SIZE) REFERENCES TARGET_SIZE(ID),
	FOREIGN KEY(ATTACK_TYPE) REFERENCES ATTACK_TYPE(ID),
	FOREIGN KEY(WEAPON_GROUP) REFERENCES WEAPON_GROUP(ID)
);

CREATE TABLE WEAPON_EXT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[EQUIPMENT_ID] INTEGER NOT NULL,
	[USAGE] INT NOT NULL,
	[WEAPON_TYPE] INTEGER NOT NULL,
	[SPEED_FACTOR] INTEGER NULL,
	[TARGET_SIZE] INTEGER NOT NULL,
	[DAMAGE_DICE_NUMBER] INTEGER NULL,
	[DAMAGE_DIE_FACES] INTEGER NULL,
	[DAMAGE_MODIFIER] INTEGER NULL,
	FOREIGN KEY(EQUIPMENT_ID) REFERENCES EQUIPMENT(ID),
	FOREIGN KEY(WEAPON_TYPE) REFERENCES WEAPON_TYPE(ID),
	FOREIGN KEY(TARGET_SIZE) REFERENCES TARGET_SIZE(ID),
	FOREIGN KEY(USAGE) REFERENCES ITEM_HAND_USAGE(ID)
);

CREATE TABLE WEAPON_PROJECTILE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[WEAPON_ID] INTEGER NOT NULL,
	[PROJECTILE_ID] INTEGER NOT NULL,
	FOREIGN KEY(WEAPON_ID) REFERENCES EQUIPMENT(ID),
	FOREIGN KEY(PROJECTILE_ID) REFERENCES EQUIPMENT(ID)
);

CREATE TABLE ARMOUR_PROTECTION_SIDE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(20) NOT NULL
);

CREATE TABLE ARMOUR
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[EQUIPMENT_ID] INTEGER NOT NULL,
	[BODY_SLOT] INTEGER NOT NULL,
	[AC_BONUS_MELEE] INTEGER NULL,
	[AC_BONUS_MISSILE] INTEGER NULL,
	[NUMBER_OF_ATTACKS_PROTECTION] INTEGER NULL,
	[PROTECTION_SIDE] INTEGER NOT NULL,
	[ALLOWS_WEAPON] CHAR(1) NULL,
	FOREIGN KEY(EQUIPMENT_ID) REFERENCES EQUIPMENT(ID)
);

CREATE TABLE WEAPON_TYPE_VS_ARMOUR_MODIFIERS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ARMOUR_ID] INTEGER NOT NULL,
	[WEAPON_TYPE_ID] INTEGER NOT NULL,
	[ARMOUR_MODIFIER] INTEGER NULL,
	FOREIGN KEY(ARMOUR_ID) REFERENCES ARMOUR(ID),
	FOREIGN KEY(WEAPON_TYPE_ID) REFERENCES WEAPON_TYPE(ID)
);

CREATE TABLE MORAL_ORDER
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[ACRONYM] VARCHAR(2) NOT NULL
);

CREATE TABLE MORAL_VIEW
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[ACRONYM] VARCHAR(2) NOT NULL
);

CREATE TABLE MORAL_ALIGNMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[ACRONYM] VARCHAR(2) NOT NULL
);

CREATE TABLE MORAL_ALIGNMENT_PER_CLASS
(
	[ALIGNMENT_ID] INTEGER NOT NULL,
	[CLASS_ID] INTEGER NOT NULL,
	PRIMARY KEY ([ALIGNMENT_ID], [CLASS_ID]),
	FOREIGN KEY(ALIGNMENT_ID) REFERENCES MORAL_ALIGNMENT(ID),
	FOREIGN KEY(CLASS_ID) REFERENCES CHARACTER_CLASS(ID)
);

CREATE TABLE PRIME_REQUISITE_BONUSES
(
	[ABILITY_SCORE] INTEGER NOT NULL PRIMARY KEY,
	[ADDITIONAL_LEVELS] INTEGER NOT NULL
);

CREATE TABLE ENCUMBRANCE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[STRENGTH_FROM] INTEGER NOT NULL,
	[EXC_STRENGTH_FROM] INTEGER NULL,
	[STRENGTH_TO] INTEGER NOT NULL,
	[EXC_STRENGTH_TO] INTEGER NULL,
	[UNENCUMBERED] INTEGER NOT NULL,
	[LIGHT] INTEGER NOT NULL,
	[MODERATE] INTEGER NOT NULL,
	[HEAVY] INTEGER NOT NULL,
	[SEVERE] INTEGER NOT NULL
);

CREATE TABLE MODIFIED_MOVEMENT_RATES
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[STRENGTH_FROM] INTEGER NOT NULL,
	[EXC_STRENGTH_FROM] INTEGER NULL,
	[STRENGTH_TO] INTEGER NOT NULL,
	[EXC_STRENGTH_TO] INTEGER NULL,
	[LOAD] FLOAT NOT NULL,
	[MOVEMENT_RATE_1] INTEGER NOT NULL,
	[MOVEMENT_RATE_2] INTEGER NOT NULL
);

CREATE TABLE ITEM_CAPABILITIES_PER_CHARACTER_CLASS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[CLASS_TYPE_ID] INTEGER NULL,
	[CLASS_ID] INTEGER NULL,
	[EQUIPMENT_ID] INTEGER NOT NULL,
	FOREIGN KEY(EQUIPMENT_ID) REFERENCES EQUIPMENT(ID)
);

CREATE TABLE DEITY_RANK
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE PLANE_RANK
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE PLANE_GROUP
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE PLANE_NATURE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL
);

CREATE TABLE PLANE_OF_EXISTENCE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[DESCRIPTION] VARCHAR(80) NOT NULL,
	[RANK] INTEGER NOT NULL,
	[GROUP] INTEGER NULL,
	[NATURE] INTEGER NOT NULL,
	FOREIGN KEY([RANK]) REFERENCES [PLANE_RANK](ID),
	FOREIGN KEY([GROUP]) REFERENCES PLANE_GROUP(ID),
	FOREIGN KEY([NATURE]) REFERENCES PLANE_NATURE(ID)
);

CREATE TABLE REALM
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(30) NOT NULL,
	[PLANE_ID] INTEGER NOT NULL,
	FOREIGN KEY([PLANE_ID]) REFERENCES PLANE_OF_EXISTENCE(ID)
);

--???TODO Valorizzare
CREATE TABLE PLANE_ALIGNMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[PLANE_ID] INTEGER NOT NULL,
	[MORAL_ALIGNMENT] INTEGER NULL,
	FOREIGN KEY([PLANE_ID]) REFERENCES PLANE_OF_EXISTENCE(ID),
	FOREIGN KEY([MORAL_ALIGNMENT]) REFERENCES [MORAL_ALIGNMENT](ID)
);

CREATE TABLE DEITY
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[RANK] INTEGER NOT NULL,
	[MORAL_ALIGNMENT] INTEGER NOT NULL,
	[REALM] INTEGER NULL,
	FOREIGN KEY([REALM]) REFERENCES [REALM](ID),
	FOREIGN KEY([MORAL_ALIGNMENT]) REFERENCES [MORAL_ALIGNMENT](ID),
	FOREIGN KEY([RANK]) REFERENCES DEITY_RANK(ID)
);

--???TODO Valorizzare
CREATE TABLE DEITY_TITLE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DEITY_ID] INTEGER NOT NULL,
	[TITLE] VARCHAR(100) NULL,
	FOREIGN KEY([DEITY_ID]) REFERENCES [DEITY](ID)
);

--???TODO Valorizzare
CREATE TABLE AREA_OF_CONTROL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(100) NULL
);

--???TODO Valorizzare
CREATE TABLE DEITY_AREA_OF_CONTROL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DEITY_ID] INTEGER NOT NULL,
	[AREA_OR_CONTROL] INTEGER NULL,
	FOREIGN KEY([DEITY_ID]) REFERENCES [DEITY](ID),
	FOREIGN KEY([AREA_OR_CONTROL]) REFERENCES [AREA_OR_CONTROL](ID)
);

CREATE TABLE WORSHIPPER_ALIGNMENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DEITY_ID] INTEGER NOT NULL,
	[MORAL_ALIGNMENT] INTEGER NULL,
	FOREIGN KEY([DEITY_ID]) REFERENCES [DEITY](ID),
	FOREIGN KEY([MORAL_ALIGNMENT]) REFERENCES [MORAL_ALIGNMENT](ID)
);

--???TODO Valorizzare
CREATE TABLE DEITY_SPHERE_OF_INFLUENCE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DEITY_ID] INTEGER NOT NULL,
	[SPHERE_ID] INTEGER NULL,
	[ACCESS_LEVEL] INTEGER NULL,
	[MODE] INTEGER NULL,
	FOREIGN KEY([DEITY_ID]) REFERENCES [DEITY](ID),
	FOREIGN KEY([SPHERE_ID]) REFERENCES [SPHERES_OF_INFLUENCE](ID),
	FOREIGN KEY([ACCESS_LEVEL]) REFERENCES [SPHERE_OF_INFLUENCE_ACCESS_LEVEL](ID),
	FOREIGN KEY([MODE]) REFERENCES [SPHERE_OF_INFLUENCE_MODE](ID)
);

--???TODO DEITY_AVATAR

CREATE TABLE MISSILE_WEAPON_RANGE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[WEAPON_ID] INTEGER NOT NULL,
	[PROJECTILE_ID] INTEGER NULL,
	[RATE_OF_FIRE_NUM_ATTACKS] INTEGER NOT NULL,
	[RATE_OF_FIRE_NUM_ROUND] INTEGER NOT NULL,
	[SHORT_DISTANCE_FROM] INTEGER NULL,
	[SHORT_DISTANCE_TO] INTEGER NOT NULL,
	[MEDIUM_DISTANCE_FROM] INTEGER NULL,
	[MEDIUM_DISTANCE_TO] INTEGER NOT NULL,
	[LONG_DISTANCE_FROM] INTEGER NULL,
	[LONG_DISTANCE_TO] INTEGER NOT NULL,
	[SHORT_DISTANCE_MODIFIER] INTEGER NOT NULL,
	[MEDIUM_DISTANCE_MODIFIER] INTEGER NOT NULL,
	[LONG_DISTANCE_MODIFIER] INTEGER NOT NULL,
	FOREIGN KEY([WEAPON_ID]) REFERENCES [EQUIPMENT](ID)
);

CREATE TABLE WIZARD_SPELL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[LEVEL] INT NOT NULL
);

CREATE TABLE PRIEST_SPELL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[LEVEL] INT NOT NULL
);

CREATE TABLE TREASURE_NATURE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL
);

CREATE TABLE TREASURE_LOCATION
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL
);

CREATE TABLE TREASURE_CLASS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ACRONYM] CHAR(1) NOT NULL,
	[LOCATION] INT NOT NULL,
	FOREIGN KEY([LOCATION]) REFERENCES [TREASURE_LOCATION](ID)
);

CREATE TABLE TREASURE_COMPONENT
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL
);

CREATE TABLE GEM_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[BASE_COIN] INT NOT NULL,
	[BASE_VALUE] INT NOT NULL,
	[PROBABILITY_FROM] INT NOT NULL,
	[PROBABILITY_TO] INT NOT NULL
);

CREATE TABLE GEMS
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[TYPE] INT NOT NULL,
	[VALUE_RANGE_COIN] INT NULL,
	[VALUE_RANGE_FROM] INT NULL,
	[VALUE_RANGE_TO] INT NULL,
	FOREIGN KEY([TYPE]) REFERENCES [GEM_TYPE](ID)
);

CREATE TABLE OBJECT_OF_ART
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[PROBABILITY_FROM] INT NULL,
	[PROBABILITY_TO] INT NULL,
	[VALUE_COIN] INT NULL,
	[VALUE_FROM] INT NULL,
	[VALUE_TO] INT NULL
);

CREATE TABLE MAGICAL_ITEM_NATURE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL
);

CREATE TABLE MAGICAL_ITEM_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL,
	[PROBABILITY_FROM] INT NOT NULL,
	[PROBABILITY_TO] INT NOT NULL
);

CREATE TABLE MAGICAL_ITEM_UNIDENTIFIED_NAME
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(50) NOT NULL
);

CREATE TABLE MAGICAL_ITEM
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[UNIDENTIFIED_ID] INT NOT NULL,
	[NAME] VARCHAR(100) NOT NULL,
	[NATURE] INT NOT NULL,
	[TYPE] INT NOT NULL,
	[XP_VALUE] INT NULL,
	[ITEM_TYPE] INT NULL,
	[EQUIPMENT_ID] INT NULL,
	FOREIGN KEY([UNIDENTIFIED_ID]) REFERENCES [MAGICAL_ITEM_UNIDENTIFIED_NAME](ID),
	FOREIGN KEY([NATURE]) REFERENCES [MAGICAL_ITEM_NATURE](ID),
	FOREIGN KEY([TYPE]) REFERENCES [MAGICAL_ITEM_TYPE](ID)
);

CREATE TABLE MAGICAL_ITEM_LIMITATION
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[ITEM_ID] INT NOT NULL,
	[ALLOWED_CLASS_ID] INT NULL,
	[ALLOWED_CLASS_TYPE_ID] INT NULL,
	[ALLOWED_ALIGNMENT_ID] INT NULL,
	FOREIGN KEY([ITEM_ID]) REFERENCES [MAGICAL_ITEM](ID)
);

CREATE TABLE MAGICAL_ITEM_PROBABILITY
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[MAGICAL_ITEM_ID] INT NOT NULL,
	[SUBTABLE_FROM] INT NOT NULL,
	[SUBTABLE_TO] INT NOT NULL,
	[PROBABILITY_FROM] INT NOT NULL,
	[PROBABILITY_TO] INT NOT NULL,
	FOREIGN KEY([MAGICAL_ITEM_ID]) REFERENCES [MAGICAL_ITEM](ID)
);

CREATE TABLE TREASURE_COMPOSITION
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[TREASURE_CLASS] INT NOT NULL,
	[COMPONENT] INT NOT NULL,
	[COUNT_FROM] INT NULL,
	[COUNT_TO] INT NULL,
	[PROBABILITY] INT NULL,
	[NATURE] INT NULL,
	[ADDITIONAL_COMPONENT] INT NULL,
	[ADDITIONAL_COUNT] INT NULL,
	FOREIGN KEY([TREASURE_CLASS]) REFERENCES [TREASURE_CLASS](ID),
	FOREIGN KEY([COMPONENT]) REFERENCES [TREASURE_COMPONENT](ID),
	FOREIGN KEY([NATURE]) REFERENCES [TREASURE_NATURE](ID),
	FOREIGN KEY([ADDITIONAL_COMPONENT]) REFERENCES [TREASURE_COMPONENT](ID)
);

CREATE TABLE MONSTER_FREQUENCY
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[DESCRIPTION] VARCHAR(100) NOT NULL,
	[CHANCE] INT NULL
);

CREATE TABLE MONSTER_INTELLIGENCE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SCORE_FROM] INT NOT NULL,
	[SCORE_TO] INT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE MONSTER_MORALE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[SCORE_FROM] INT NOT NULL,
	[SCORE_TO] INT NULL,
	[DESCRIPTION] VARCHAR(30) NOT NULL
);

CREATE TABLE MONSTER
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] VARCHAR(100) NOT NULL,
	[CLIMATE] INT NULL,
	[TERRAIN] INT NULL,
	[FREQUENCY] INT NULL,
	[ORGANISATION] INT NULL,
	[ACTIVITY_CYCLE] INT NULL,
	[DIET] INT NULL,
	[INTELLIGENCE] INT NULL,
	[ALIGNMENT] INT NULL,
	[MORALE] INT NULL
);

CREATE TABLE MONSTER_UNDEAD
(
	[MONSTER_ID] INTEGER NOT NULL PRIMARY KEY,
	[TURNED_AS] INT NULL,
	FOREIGN KEY([MONSTER_ID]) REFERENCES [MONSTER](ID),
	FOREIGN KEY([TURNED_AS]) REFERENCES [TURNABLE](ID)
);

CREATE TABLE MONSTER_ELEMENTAL
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[MONSTER_ID] INTEGER NOT NULL,
	[ELEMENT_ID] INT NULL,
	FOREIGN KEY([MONSTER_ID]) REFERENCES [MONSTER](ID),
	FOREIGN KEY([ELEMENT_ID]) REFERENCES [ELEMENT](ID)
);

CREATE TABLE MONSTER_DRAGON_TYPE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[NAME] INTEGER NOT NULL
);

CREATE TABLE MONSTER_DRAGON
(
	[MONSTER_ID] INTEGER NOT NULL PRIMARY KEY,
	[TYPE] INT NULL,
	FOREIGN KEY([TYPE]) REFERENCES [MONSTER_DRAGON_TYPE](ID)
);

CREATE TABLE MONSTER_TREASURE
(
	[ID] INTEGER NOT NULL PRIMARY KEY,
	[MONSTER_ID] INTEGER NOT NULL,
	[TREASURE_LOCATION_ID] INT NOT NULL,
	[TREASURE_CLASS_ID] INT NOT NULL,
	[MULTIPLIER] INT NULL,
	FOREIGN KEY([TREASURE_LOCATION_ID]) REFERENCES [TREASURE_LOCATION](ID),
	FOREIGN KEY([TREASURE_CLASS_ID]) REFERENCES [TREASURE_CLASS](ID)
);