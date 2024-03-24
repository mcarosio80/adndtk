CREATE UNIQUE INDEX [IDX_SKILL_MODIFIER] ON [SKILL_MODIFIER] ([RACE_ID] ASC, [SKILL_ID] ASC);
CREATE UNIQUE INDEX [IDX_PRIMARY_SKILL] ON [PRIMARY_SKILL] ([CLASS_ID] ASC, [SKILL_ID] ASC);
CREATE UNIQUE INDEX [IDX_TURNABLE] ON [TURNABLE] ([NAME] ASC);
CREATE UNIQUE INDEX [IDX_TURN_EFFECT] ON [TURN_EFFECT] ([ACRONYM] ASC);
CREATE UNIQUE INDEX [IDX_TURN_UNDEAD] ON [TURN_UNDEAD] ([TURNABLE_CODE] ASC, [LEVEL] ASC);
CREATE UNIQUE INDEX [IDX_SAVING_THROW] ON [SAVING_THROW] ([DESCRIPTION] ASC);
CREATE UNIQUE INDEX [IDX_SAVING_THROW_SCORES] ON [SAVING_THROW_SCORES] ([CLASS_TYPE_ID] ASC, [LEVEL] ASC, [SAVING_THROW_ID] ASC);
CREATE UNIQUE INDEX [IDX_THIEF_ABILITY] ON [THIEF_ABILITY] ([NAME] ASC);
CREATE UNIQUE INDEX [IDX_THIEF_ABILITY_SCORES] ON [THIEF_ABILITY_SCORES] ([LEVEL] ASC, [THIEF_ABILITY_ID] ASC);
--CREATE UNIQUE INDEX [IDX_LEVEL_ADVANCEMENT] ON [LEVEL_ADVANCEMENT] ([CLASS_ID] ASC, [LEVEL] ASC);
CREATE UNIQUE INDEX [IDX_CLASS_AVAILABILITY] ON [CLASS_AVAILABILITY] ([RACE_ID] ASC, [CLASS_ID] ASC);
CREATE UNIQUE INDEX [IDX_SCHOOL_OF_MAGIC_OPPOSITION_1] ON [SCHOOL_OF_MAGIC_OPPOSITION] ([SCHOOL_ID] ASC, [OPPOSITE_ID] ASC);
CREATE UNIQUE INDEX [IDX_THIEVING_SKILL_RACIAL_ADJUSTMENTS] ON [THIEVING_SKILL_RACIAL_ADJUSTMENTS] ([THIEVING_SKILL] ASC, [RACE_ID] ASC);
CREATE UNIQUE INDEX [IDX_THIEVING_SKILL_DEXTERITY_ADJUSTMENTS] ON [THIEVING_SKILL_DEXTERITY_ADJUSTMENTS] ([DEXTERITY] ASC, [THIEVING_SKILL] ASC);
CREATE UNIQUE INDEX [IDX_COIN] ON [COIN] ([ACRONYM] ASC);
CREATE UNIQUE INDEX [IDX_COIN_EXCHANGE_VALUES] ON [COIN_EXCHANGE_VALUES] ([COIN_FROM] ASC, [COIN_TO] ASC);
CREATE INDEX [IDX_CALENDAR_MONTH] ON [CALENDAR_MONTH] ([CALENDAR_ID] ASC);
CREATE INDEX [IDX_CALENDAR_RECKONING] ON [CALENDAR_RECKONING] ([CALENDAR_ID] ASC);