#include <racial_stats.h>
#include <cyclopedia.h>
#include <dice.h>


Adndtk::RacialStats::RacialStats()
	: _race{Defs::race::human}, _currentAge{0},
	_middleAge{0}, _oldAge{0}, _venerableAge{0}, _startingAge{0}, _maxAge{0}, _height{0}, _weight{0}
{
}

Adndtk::RacialStats::RacialStats(const Defs::race& race, const Defs::sex& sex)
	: _race{race}, _sex{sex}, _currentAge{0},
	_middleAge{0}, _oldAge{0}, _venerableAge{0}, _startingAge{0}, _maxAge{0}, _height{0}, _weight{0}
{
	auto raceId = static_cast<int>(_race);
	auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_racial_stats, raceId);
	auto& stats = rs[0];

	auto baseHeightMale = stats.as<uint16_t>("height_base_male");
	auto baseHeightFemale = stats.as<uint16_t>("height_base_female");
	auto baseHeightDiceNum1 = stats.as<uint16_t>("height_dice_number");
	auto baseHeightDiceFaces1 = stats.as<uint16_t>("height_dice_faces");
	auto baseHeightDiceNum2 = stats.as<uint16_t>("height_dice_number_2");
	auto baseHeightDiceFaces2 = stats.as<uint16_t>("height_dice_faces_2");
	_height = Die::roll(baseHeightDiceNum1, baseHeightDiceFaces1, 0)
					+ Die::roll(baseHeightDiceNum2, baseHeightDiceFaces2, 0)
					+ (_sex == Defs::sex::male) ? baseHeightMale : baseHeightFemale;

	auto baseWeightMale = stats.as<uint16_t>("weight_base_male");
	auto baseWeightFemale = stats.as<uint16_t>("weight_base_female");
	auto baseWeightDiceNum1 = stats.as<uint16_t>("weight_dice_number");
	auto baseWeightDiceFaces1 = stats.as<uint16_t>("weight_dice_faces");
	auto baseWeightDiceNum2 = stats.as<uint16_t>("weight_dice_number_2");
	auto baseWeightDiceFaces2 = stats.as<uint16_t>("weight_dice_faces_2");
	_weight = Die::roll(baseWeightDiceNum1, baseWeightDiceFaces1, 0)
					+ Die::roll(baseWeightDiceNum2, baseWeightDiceFaces2, 0)
					+ (_sex == Defs::sex::male) ? baseWeightMale : baseWeightFemale;

	auto startingAge = stats.as<int>("age_starting");
	auto startingAgeDiceNum = stats.as<int>("age_dice_number");
	auto startingAgeDiceFaces = stats.as<int>("age_dice_faces");
	_startingAge = Die::roll(startingAgeDiceNum, startingAgeDiceFaces, startingAge);

	auto maxAge = stats.as<int>("age_maximum");
	auto maxAgeDiceNum = stats.as<int>("age_maximum_dice_number");
	auto maxAgeDiceFaces = stats.as<int>("age_maximum_dice_faces");
	_maxAge = Die::roll(maxAgeDiceNum, maxAgeDiceFaces, maxAge);

	_middleAge = stats.as<uint16_t>("middle_age");
	_oldAge = stats.as<uint16_t>("old_age");
	_venerableAge = stats.as<uint16_t>("venerable_age");
}

Adndtk::RacialStats::~RacialStats()
{
}

bool Adndtk::RacialStats::grow_old(const uint16_t& years)
{
	if (_currentAge + years > _maxAge)
	{
		return false;
	}
	auto prevAgeRange = get_age_range();
	_currentAge += years;
	auto currAgeRange = get_age_range();

	if (currAgeRange > prevAgeRange)
	{
		auto modifiers = get_skill_modifiers(prevAgeRange, currAgeRange);
		for (auto& mod : modifiers)
		{
			if (mod.second != 0)
			{
				for (auto& cbk : _sklChgCbk)
				{
					cbk(mod.first, mod.second);
				}
			}
		}
	}

	return true;
}

bool Adndtk::RacialStats::rejuvenate(const uint16_t& years)
{
	if (_currentAge - years < _startingAge)
	{
		return false;
	}
	auto prevAgeRange = get_age_range();
	_currentAge -= years;
	auto currAgeRange = get_age_range();

	if (currAgeRange < prevAgeRange)
	{
		auto modifiers = get_skill_modifiers(currAgeRange, prevAgeRange);
		for (auto& mod : modifiers)
		{
			if (mod.second != 0)
			{
				for (auto& cbk : _sklChgCbk)
				{
					cbk(mod.first, -mod.second);
				}
			}
		}
	}

	return true;
}

Adndtk::Defs::aging_effects Adndtk::RacialStats::get_age_range() const
{
	auto ageRange{Defs::aging_effects::young};

	if (_currentAge > _maxAge)
	{
		throw std::runtime_error("Current age cannot be greater than maximum age");
	}
	else if (_currentAge >= _venerableAge)
	{
		return Defs::aging_effects::venerable;
	}
	else if (_currentAge >= _oldAge)
	{
		return Defs::aging_effects::old_age;
	}
	else if (_currentAge >= _middleAge)
	{
		return Defs::aging_effects::middle_age;
	}
	else if (_currentAge >= _startingAge)
	{
		return Defs::aging_effects::young;
	}
	else
	{
		throw std::runtime_error("Current age cannot be smaller than starting age");
	}
	return ageRange;
}

std::map<Adndtk::Defs::skill, short> Adndtk::RacialStats::get_skill_modifiers(const Defs::aging_effects& ageRangeFrom, const Defs::aging_effects& ageRangeTo) const
{
	auto rs = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_aging_effects);

	short strModifier = 0;
	short dexModifier = 0;
	short conModifier = 0;
	short intModifier = 0;
	short wisModifier = 0;
	short chaModifier = 0;

	for (auto& r : rs)
	{
		auto id = static_cast<Defs::aging_effects>(r.as<int>("id"));
		if (id > ageRangeFrom && id <= ageRangeTo)
		{
			strModifier += r.as<short>("strength_modifier");
			dexModifier += r.as<short>("dexterity_modifier");
			conModifier += r.as<short>("constitution_modifier");
			intModifier += r.as<short>("intelligence_modifier");
			wisModifier += r.as<short>("wisdom_modifier");
			chaModifier += r.as<short>("charisma_modifier");
		}
	}

	std::map<Defs::skill, short> res
	{
		{Defs::skill::strength, strModifier},
		{Defs::skill::dexterity, dexModifier},
		{Defs::skill::constitution, conModifier},
		{Defs::skill::intelligence, intModifier},
		{Defs::skill::wisdom, wisModifier},
		{Defs::skill::charisma, chaModifier}
	};
	return res;
}