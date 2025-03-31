# Adndtk - BACKLOG
 
List of the new features, changes to existing features, bug fixes, infrastructure changes or other activities that might eventually be delivered.

## New features
  * Account for non movable objects (boat, live cattle, horse, etc)
  * Account for encumbrance of treasure items
  * Monsters
  * Implement ledger of buy/sell transaction for a store
  * Implement library telemetry (log method calls and effects)
  * Implement battle field and account for position of characters/monsters and objects on it
  * Detect object included into the Area of Effect (to be used for spells)
  * Add Event Scheduler to control event sequence and time (to be used for spells)
  * Fix name generation for enum class magical_item (+1 or similar objects)
  * Add place of birth in character generation in CLI tool
  * CLI: Handle demihumans with skill scores = 19, resulting in no classes available
  * Check notes for Necromantic in DEITY_SPHERE_OF_INFLUENCE
  * Insert data into PLANE_ALIGNMENT
  * Insert data into DEITY_TITLE
  * Insert data into AREA_OF_CONTROL
  * Insert data into DEITY_AREA_OF_CONTROL
  * Insert data into DEITY_SPHERE_OF_INFLUENCE
  * Insert data into DEITY_AVATAR
  * Generate static headers with map data from SQL tables
  * Check test failure [TC-CHAR.043] due to unexpected exception with message: map::at
  * Check test failure [TC-PROF.001] at REQUIRE( slots.get_base_weapon_initial_score() == 1 ) with expansion: 2 == 1
        <br><code>
        REQUIRE(ProficiencySlots(Defs::character_class::necromancer).get_base_weapon_initial_score() == 1);
        </code>
  * Check: no attacks may cause less than 1 damage (pag 13 PHB)
  
## Planned changes

## Bug fixes

# Missing unit-test