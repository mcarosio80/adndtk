#include <racial_stats.h>
#include <cyclopedia.h>
#include <dice.h>
#include <options.h>


Adndtk::RacialStats::RacialStats()
	: _race{Defs::race::human}, _currentAge{0},
	_middleAge{0}, _oldAge{0}, _venerableAge{0}, _startingAge{0}, _maxAge{0}, _height{0}, _weight{0}
{
}

Adndtk::RacialStats::RacialStats(const Defs::race& race, const Defs::sex& sex)
	: _race{race}, _sex{sex}, _currentAge{0},
	_middleAge{0}, _oldAge{0}, _venerableAge{0}, _startingAge{0}, _maxAge{0}, _height{0}, _weight{0}
{
	auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::race>(Query::select_racial_stats, _race);
	auto& stats = rs[0];

	auto baseHeightMale = stats.as<short>("height_base_male");
	auto baseHeightFemale = stats.as<short>("height_base_female");
	auto baseHeightDiceNum1 = stats.as<short>("height_dice_number");
	auto baseHeightDiceFaces1 = stats.as<Defs::die>("height_dice_faces");
	auto baseHeightDiceNum2 = stats.try_as<short>("height_dice_number_2");
	auto baseHeightDiceFaces2 = stats.try_as<short>("height_dice_faces_2");
	_height = DiceSet::get_instance().roll(baseHeightDiceNum1, baseHeightDiceFaces1, 0);
	_height += (_sex == Defs::sex::male) ? baseHeightMale : baseHeightFemale;
	if (baseHeightDiceNum2.has_value())
	{
		for (short n{0}; n < baseHeightDiceNum2.value(); ++n)
		{
			_height += Die::roll<short>(1, baseHeightDiceFaces2.value());
		}
	}

	auto baseWeightMale = stats.as<short>("weight_base_male");
	auto baseWeightFemale = stats.as<short>("weight_base_female");
	auto baseWeightDiceNum1 = stats.as<short>("weight_dice_number");
	auto baseWeightDiceFaces1 = stats.as<Defs::die>("weight_dice_faces");
	auto baseWeightDiceNum2 = stats.try_as<short>("weight_dice_number_2");
	auto baseWeightDiceFaces2 = stats.try_as<short>("weight_dice_faces_2");
	_weight = DiceSet::get_instance().roll(baseWeightDiceNum1, baseWeightDiceFaces1, 0);
	_weight += (_sex == Defs::sex::male) ? baseWeightMale : baseWeightFemale;
	if (baseWeightDiceNum2.has_value())
	{
		for (short n{0}; n < baseWeightDiceNum2.value(); ++n)
		{
			_weight += Die::roll<short>(1, baseWeightDiceFaces2.value());
		}
	}

	auto startingAge = stats.as<int>("age_starting");
	auto startingAgeDiceNum = stats.as<int>("age_dice_number");
	auto startingAgeDiceFaces = stats.as<Defs::die>("age_dice_faces");
	_startingAge = DiceSet::get_instance().roll(startingAgeDiceNum, startingAgeDiceFaces, startingAge);
	_currentAge = _startingAge;

	auto maxAge = stats.as<int>("age_maximum");
	auto maxAgeDiceNum = stats.as<int>("age_maximum_dice_number");
	auto maxAgeDiceFaces = stats.as<Defs::die>("age_maximum_dice_faces");
	_maxAge = DiceSet::get_instance().roll(maxAgeDiceNum, maxAgeDiceFaces, maxAge);

	_middleAge = stats.as<short>("middle_age");
	_oldAge = stats.as<short>("old_age");
	_venerableAge = stats.as<short>("venerable_age");
}

Adndtk::RacialStats::~RacialStats()
{
}

bool Adndtk::RacialStats::grow_old(const short& years)
{
	if (_currentAge + years > _maxAge)
	{
		return false;
	}
	auto prevAgeRange = get_age_range();
	_currentAge += years;
	auto currAgeRange = get_age_range();

	if (currAgeRange > prevAgeRange && OptionalRules::get_instance().get_option<bool>(Option::apply_aging_effect))
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

bool Adndtk::RacialStats::rejuvenate(const short& years)
{
	if (_currentAge - years < _startingAge)
	{
		return false;
	}
	auto prevAgeRange = get_age_range();
	_currentAge -= years;
	auto currAgeRange = get_age_range();

	if (currAgeRange < prevAgeRange && OptionalRules::get_instance().get_option<bool>(Option::apply_aging_effect))
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
		auto id = r.as<Defs::aging_effects>("id");
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