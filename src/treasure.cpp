#include <treasure.h>
#include <cyclopedia.h>
#include <dice.h>

#include <math.h>
#include <ranges>
#include <vector>
#include <iterator>
#include <tuple>


bool Adndtk::TreasurePool::_initialised = false;

using TreasureInfo = std::tuple<short, uint32_t, uint32_t, int, std::optional<int>, std::optional<int>, std::optional<int>>;

Adndtk::TreasurePool::TreasurePool()
    : _lastItemId{0}, _rd{}, _generator{_rd()}, _uniformDistribution{_valuesFrom, _valuesTo}
{
}

Adndtk::TreasurePool& Adndtk::TreasurePool::get_instance()
{
	static TreasurePool _instance;

    if (!_initialised)
	{   
        _initialised = true;
	}
	return _instance;
}

Adndtk::Coin Adndtk::TreasurePool::generate_value(const Defs::coin& coin, const uint32_t& valueFrom, const uint32_t& valueTo)
{
    auto rv = static_cast<double>(_uniformDistribution(_generator));
    auto diff = valueTo - valueFrom;
    double ratio = rv / _valuesTo;
    double v = diff * ratio;
    uint32_t val = std::round(v + valueFrom);
    return {coin, std::min(std::max(val, valueFrom), valueTo)};
}

Adndtk::Coin Adndtk::TreasurePool::generate_value(const std::optional<Defs::coin>& rangeCoin, const std::optional<uint32_t>& valueFrom, const std::optional<uint32_t>& valueTo, const Defs::coin& baseCoin, const uint32_t& baseValue)
{
    if (!rangeCoin.has_value())
    {
        return {baseCoin, baseValue};
    }

	return generate_value(rangeCoin.value(), valueFrom.value(), valueTo.value());
}

Adndtk::Defs::gem Adndtk::TreasurePool::pick_gem()
{
	auto d = Die{Defs::die::d100};
	auto prob = d.roll();

    auto getTypeInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_gem_type, prob);
    auto gemType = static_cast<Defs::gem_type>(getTypeInfo[0].as<int>("id"));

    auto t = static_cast<int>(gemType);
    auto gemIds = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_gems_by_type, t);

    auto index = _uniformDistribution(_generator) % gemIds.size();
    auto g = gemIds[index].as<int>("id");

	return static_cast<Defs::gem>(g);
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_magical()
{
	auto d = Die{Defs::die::d20};
	auto prob = d.roll();

    auto info = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_magical_item_type_by_probability, prob);
    auto magicalItemType = static_cast<Defs::magical_item_type>(info[0].as<int>("id"));

	Defs::magical_item magicalItem{};
	if (magicalItemType == Defs::magical_item_type::potions_and_oils)
	{
		magicalItem = pick_potion();
	}
	else if (magicalItemType == Defs::magical_item_type::scrolls)
	{
		magicalItem = pick_scroll();
	}
	else if (magicalItemType == Defs::magical_item_type::rings)
	{
		magicalItem = pick_ring();
	}
	else if (magicalItemType == Defs::magical_item_type::rods)
	{
		magicalItem = pick_rod();
	}
	else if (magicalItemType == Defs::magical_item_type::staves)
	{
		magicalItem = pick_stave();
	}
	else if (magicalItemType == Defs::magical_item_type::wands)
	{
		magicalItem = pick_wand();
	}
	else if (magicalItemType == Defs::magical_item_type::books_and_tomes)
	{
		magicalItem = pick_book();
	}
	else if (magicalItemType == Defs::magical_item_type::jewels_and_jewelry)
	{
		magicalItem = pick_jewel();
	}
	else if (magicalItemType == Defs::magical_item_type::cloaks_and_robes)
	{
		magicalItem = pick_cloak();
	}
	else if (magicalItemType == Defs::magical_item_type::boots_and_gloves)
	{
		magicalItem = pick_boot();
	}
	else if (magicalItemType == Defs::magical_item_type::girdles_and_helms)
	{
		magicalItem = pick_girdle();
	}
	else if (magicalItemType == Defs::magical_item_type::bags_and_bottles)
	{
		magicalItem = pick_bag();
	}
	else if (magicalItemType == Defs::magical_item_type::dusts_and_stones)
	{
		magicalItem = pick_dust();
	}
	else if (magicalItemType == Defs::magical_item_type::household_items_and_tools)
	{
		magicalItem = pick_household();
	}
	else if (magicalItemType == Defs::magical_item_type::musical_instruments)
	{
		magicalItem = pick_musical();
	}
	else if (magicalItemType == Defs::magical_item_type::the_wierd_stuff)
	{
		magicalItem = pick_wierd();
	}
	else if (magicalItemType == Defs::magical_item_type::armour_and_shields)
	{
		magicalItem = pick_armour();
	}
	else if (magicalItemType == Defs::magical_item_type::weapons)
	{
		magicalItem = pick_weapon();
	}

	return magicalItem;
}


Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_potion()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::potions_and_oils);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_scroll()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto itemIndex = (subtableIndex >= 5) ? Die::roll(2, 19) : Die::roll(1, 100);
	
	auto itemType = static_cast<int>(Defs::magical_item_type::scrolls);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_ring()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::rings);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_rod()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::rods);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_stave()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::staves);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_wand()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::wands);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_book()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::books_and_tomes);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_jewel()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::jewels_and_jewelry);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_cloak()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::cloaks_and_robes);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_boot()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::boots_and_gloves);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_girdle()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::girdles_and_helms);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_bag()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::bags_and_bottles);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_dust()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::dusts_and_stones);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_household()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::household_items_and_tools);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_musical()
{
	auto subtableIndex = 1;
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::musical_instruments);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_wierd()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto itemIndex = Die::roll(1, 19);
	auto itemType = static_cast<int>(Defs::magical_item_type::the_wierd_stuff);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_armour()
{
	auto d20 = Die{Defs::die::d20};
	auto itemIndex = d20.roll();
	auto subtableIndex = 1;
	
	if (itemIndex == 20)
	{
		itemIndex = d20.roll();
		subtableIndex = 2;
	}

	auto itemType = static_cast<int>(Defs::magical_item_type::armour_and_shields);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}

Adndtk::Defs::magical_item Adndtk::TreasurePool::pick_weapon()
{
	auto d6 = Die{Defs::die::d6};
	auto subtableIndex = d6.roll();
	auto d20 = Die{Defs::die::d20};
	auto itemIndex = d20.roll();

	if (itemIndex == 20)
	{
		itemIndex = Die::roll(1, 19);
		subtableIndex = Die::roll(11, 20);
	}

	auto itemType = static_cast<int>(Defs::magical_item_type::weapons);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int, int, int>(Query::select_magical_item_by_type_and_probability, itemType, subtableIndex, itemIndex);
    return static_cast<Defs::magical_item>(info[0].as<int>("id"));
}





Adndtk::Gem::Gem()
    : Gem(TreasurePool::get_instance().pick_gem())
{
}

Adndtk::Gem::Gem(const Defs::gem& gemId)
    : _uid{TreasurePool::get_instance().get_next_id()}, _gemId{gemId}, _value{Defs::coin::gold_piece, 0}
{
    int id = static_cast<int>(_gemId);
    auto gemInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_gem, id);
	
	auto& i = gemInfo[0];
	_name = i.as<std::string>("name");
	_gemType = static_cast<Defs::gem_type>(i.as<int>("type"));

	auto v = i.try_as<int>("value_range_coin");
	auto rangeCoin = (v.has_value()) ? std::make_optional(static_cast<Defs::coin>(v.value())) : std::nullopt;
	auto rangeFrom = i.try_as<uint32_t>("value_range_from");
	auto rangeTo = i.try_as<uint32_t>("value_range_to");
	auto baseCoin = static_cast<Defs::coin>(i.as<int>("base_coin"));
	auto baseValue = i.as<uint32_t>("base_value");

	_value = TreasurePool::get_instance().generate_value(rangeCoin, rangeFrom, rangeTo, baseCoin, baseValue);
}

Adndtk::ObjectOfArt::ObjectOfArt(const std::optional<std::string> name/*=std::nullopt*/)
    : _objectType{0},
	  _uid{TreasurePool::get_instance().get_next_id()},
	  _name{name.value_or("")},
	  _value{Defs::coin::gold_piece, 0}
{
	if (_name.empty())
	{
		std::stringstream ss;
		ss << "Object of art " << _uid;
		_name = ss.str();
	}

	auto d = Die{Defs::die::d100};
	auto prob = d.roll();
    auto objInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_object_of_art_by_probability, prob);

	auto& i = objInfo[0];

	_objectType = i.as<short>("id");
	auto coin = static_cast<Defs::coin>(i.as<short>("value_coin"));
	auto valueFrom = i.as<int>("value_from");
	auto valueTo = i.as<int>("value_to");

	_value = TreasurePool::get_instance().generate_value(coin, valueFrom, valueTo);
}

Adndtk::ObjectOfArt::~ObjectOfArt()
{
}


Adndtk::MagicalItem::MagicalItem()
    : MagicalItem(TreasurePool::get_instance().pick_magical())
{
}

Adndtk::MagicalItem::MagicalItem(const Defs::magical_item& itemId)
	: _id{itemId},
      _uid{TreasurePool::get_instance().get_next_id()},
      _name{}, _unidentifiedName{},
      _nature{},
      _type{},
      _xpValue{std::nullopt},
      _itemType{},
      _equipmentId{std::nullopt},
	  _identified{false}
{
	int id = static_cast<int>(_id);
    auto info = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_magical_item, id);
	
	auto& i = info[0];
	_name = i.as<std::string>("name");
	_nature = static_cast<Defs::magical_item_nature>(i.as<int>("nature"));
	_type = static_cast<Defs::magical_item_type>(i.as<int>("type"));
	_xpValue = static_cast<XP>(i.try_or<uint64_t>("xp_value", 0));
	_itemType = static_cast<Defs::equipment_type>(i.as<int>("item_type"));
	auto v = i.try_as<int>("equipment_id");
	auto rangeCoin = (v.has_value()) ? std::make_optional(static_cast<Defs::equipment>(v.value())) : std::nullopt;
	_unidentifiedName = i.as<std::string>("unidentified_name");

	auto limits = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_magical_item_limitations, id);
	if (limits.size() > 0)
	{
		auto& l = limits[0];
		auto allowedClasses = l.try_as<int>("allowed_class_id");
		auto classes = Cyclopedia::get_instance().split<int>(allowedClasses.value_or(0));
		for (auto& c : classes)
		{
			auto cls = static_cast<Defs::character_class>(c);
			_allowedClasses.emplace(cls);
		}

		auto allowedClassTypes = l.try_as<int>("allowed_class_type_id");
		auto clsTypes = Cyclopedia::get_instance().split<int>(allowedClassTypes.value_or(0));
		for (auto& ct : clsTypes)
		{
			auto cType = static_cast<Defs::character_class_type>(ct);
			_allowedClassTypes.emplace(cType);
		}

		auto allowedAlignments = l.try_as<int>("allowed_alignment_id");
		auto aligns = Cyclopedia::get_instance().split<int>(allowedAlignments.value_or(0));
		for (auto& a : aligns)
		{
			auto align = static_cast<Defs::moral_alignment>(a);
			_allowedAlignments.emplace(align);
		}
	}
}

Adndtk::MagicalItem::~MagicalItem()
{
}

const auto& Adndtk::MagicalItem::name() const
{
	return (_identified) ? _name : _unidentifiedName;
}

auto Adndtk::MagicalItem::nature() const
{
	return (_identified) ? _nature : Defs::magical_item_nature::neutral;
}


Adndtk::Treasure::Treasure(const Defs::treasure_class& cls)
	: _uid{TreasurePool::get_instance().get_next_id()},
      _class{cls},
      _coins{},
      _gems{},
      _objectsOfArt{},
      _magicalItems{}
{
	auto treasCls = static_cast<short>(cls);
    auto components = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_treasure_components);

	auto compIds = components.to_vector<short>("id");

	Die d100{Defs::die::d100};
    std::vector<TreasureInfo> treasureInfo;

	for (auto& compId : compIds)
	{
		auto treasureComposition = Cyclopedia::get_instance().exec_prepared_statement<short, short>(Query::select_treasure_composition, treasCls, compId);

		if (treasureComposition.size() > 0)
		{
			auto& t = treasureComposition[0];
			auto countFrom = t.as<uint32_t>("count_from");
			auto countTo = t.as<uint32_t>("count_to");
			auto probability = t.as<int>("probability");
			auto nature = t.try_as<int>("nature");
			auto additionalComponent = t.try_as<int>("additional_component");
			auto additionalCount = t.try_as<int>("additional_count");

			if (d100.roll() <= probability)
			{
				auto info = std::make_tuple(compId, countFrom, countTo, probability, nature, additionalComponent, additionalCount);
				treasureInfo.push_back(info);
			}
		}
	}

	for (auto& i : treasureInfo)
	{
		auto compId = std::get<0>(i);
		auto countFrom = std::get<1>(i);
		auto countTo = std::get<2>(i);
		auto probability = std::get<3>(i);
		auto nature = std::get<4>(i);
		auto additionalComponent = std::get<5>(i);
		auto additionalCount = std::get<6>(i);

		auto tc = static_cast<Defs::treasure_component>(compId);
		if (tc == Defs::treasure_component::copper)
		{
			add_coins(Defs::coin::copper_piece, countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::silver)
		{
			add_coins(Defs::coin::silver_piece, countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::gold)
		{
			add_coins(Defs::coin::gold_piece, countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::platinum_or_electrum)
		{
			auto coin = (d100.roll() % 2 == 0) ? Defs::coin::platinum_piece : Defs::coin::electrum_piece;
			add_coins(coin, countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::gems)
		{
			add_gem(countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::art_objects)
		{
			add_object_of_art(countFrom, countTo);
		}
		else if (tc == Defs::treasure_component::magical_items
				|| tc == Defs::treasure_component::magical_items_additional)
		{
			add_magical_item(countFrom, countTo, nature.value(), additionalComponent, additionalCount);
		}
	}
}

void Adndtk::Treasure::add_coins(const Defs::coin& coin, const uint32_t& countFrom, const uint32_t& countTo)
{
	auto t = TreasurePool::get_instance().generate_value(coin, countFrom, countTo);
	(*this) += t;
}

void Adndtk::Treasure::add_gem(const uint32_t& countFrom, const uint32_t& countTo)
{
	auto gemsCount = (countFrom == countTo) ? countFrom : Die::roll(countFrom, countTo);
	for (int i=0; i<gemsCount; ++i)
	{
		(*this) += Gem{};
	}
}

void Adndtk::Treasure::add_object_of_art(const uint32_t& countFrom, const uint32_t& countTo)
{
	auto objOfArtCount = (countFrom == countTo) ? countFrom : Die::roll(countFrom, countTo);
	for (int i=0; i<objOfArtCount; ++i)
	{
		(*this) += ObjectOfArt{};
	}
}

std::vector<Adndtk::Defs::magical_item_type> Adndtk::Treasure::get_magical_types(const int& nature)
{
	std::vector<Defs::magical_item_type> magicalItemTypes{};

	if (nature & static_cast<int>(Defs::treasure_nature::magical_weapon))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::weapons);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::magical_armour))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::armour_and_shields);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::potions_and_oils))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::potions_and_oils);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::scrolls))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::scrolls);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::rings))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::rings);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::wands_staves_and_rods))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::wands);
		magicalItemTypes.push_back(Defs::magical_item_type::staves);
		magicalItemTypes.push_back(Defs::magical_item_type::rods);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::miscellaneous))
	{
		magicalItemTypes.push_back(Defs::magical_item_type::books_and_tomes);
		magicalItemTypes.push_back(Defs::magical_item_type::jewels_and_jewelry);
		magicalItemTypes.push_back(Defs::magical_item_type::cloaks_and_robes);
		magicalItemTypes.push_back(Defs::magical_item_type::boots_and_gloves);
		magicalItemTypes.push_back(Defs::magical_item_type::girdles_and_helms);
		magicalItemTypes.push_back(Defs::magical_item_type::bags_and_bottles);
		magicalItemTypes.push_back(Defs::magical_item_type::dusts_and_stones);
		magicalItemTypes.push_back(Defs::magical_item_type::household_items_and_tools);
		magicalItemTypes.push_back(Defs::magical_item_type::musical_instruments);
		magicalItemTypes.push_back(Defs::magical_item_type::the_wierd_stuff);
	}
	if (nature & static_cast<int>(Defs::treasure_nature::artifacts_and_relics))
	{
	}

	return magicalItemTypes;
}

void Adndtk::Treasure::add_magical_item(const uint32_t& countFrom, const uint32_t& countTo, const int& nature, const std::optional<int>& additionalComponent, const std::optional<int>& additionalCount)
{
	auto count = (countFrom == countTo) ? countFrom : Die::roll(countFrom, countTo);

	for (unsigned int i=0; i<count; ++i)
	{
		auto magicalItemTypes = get_magical_types(nature);
		if (!magicalItemTypes.empty())
		{
			auto magicalType = select_one<Defs::magical_item_type>(magicalItemTypes);
			auto magicalsByType = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_magical_items_by_type, static_cast<int>(magicalType));
			auto ids = magicalsByType.to_vector<int>("id");
			auto magicalId = static_cast<Defs::magical_item>(select_one<int>(ids));
			MagicalItem item{magicalId};
			_magicalItems.push_back(item);
		}
	}
}

Adndtk::Treasure::~Treasure()
{
}

Adndtk::Treasure& Adndtk::Treasure::operator+=(const Adndtk::Coin& amt)
{
	_coins.add(amt);
	return (*this);
}

Adndtk::Treasure& Adndtk::Treasure::operator+=(const Gem& g)
{
	_gems.push_back(g);
	return (*this);
}

Adndtk::Treasure& Adndtk::Treasure::operator+=(const ObjectOfArt& o)
{
	_objectsOfArt.push_back(o);
	return (*this);
}

Adndtk::Treasure& Adndtk::Treasure::operator+=(const MagicalItem& m)
{
	_magicalItems.push_back(m);
	return (*this);
}

std::set<Adndtk::Defs::coin> Adndtk::Treasure::coins()
{
	std::set<Defs::coin> result;

	for (auto c : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece,
					Defs::coin::gold_piece, Defs::coin::platinum_piece })
	{
		if (_coins[c] > 0)
		{
			result.emplace(c);
		}
	}
	return result;
}

Adndtk::Coin Adndtk::Treasure::value(const Defs::coin& currency)
{
	auto value = _coins[currency];
	return {currency, value};
}

const std::vector<Adndtk::Gem>& Adndtk::Treasure::gems()
{
	return _gems;
}

const std::vector<Adndtk::ObjectOfArt>&	Adndtk::Treasure::objects_of_art()
{
	return _objectsOfArt;
}

const std::vector<Adndtk::MagicalItem>&	Adndtk::Treasure::magical_items()
{
	return _magicalItems;
}

double Adndtk::Treasure::total_value(const Defs::coin& currency)
{
	double treasureValue = 0.0;

	for (auto& c : coins())
	{
		auto val = _coins[c];
		if (val > 0)
		{
			double fx = CoinExchange::get_instance().get_conversion_ratio(c, currency);
			treasureValue += val * fx;
		}
	}
	for (auto g : _gems)
	{
		Coin amt = g.value();
		double fx = CoinExchange::get_instance().get_conversion_ratio(amt.currency(), currency);
		treasureValue += amt.value() * fx;
	}
	for (auto o : _objectsOfArt)
	{
		Coin amt = o.value();
		double fx = CoinExchange::get_instance().get_conversion_ratio(amt.currency(), currency);
		treasureValue += amt.value() * fx;
	}
	return treasureValue;
}

// const adnd::treasure::treasure& adnd::treasure::treasure_pool::create(const defs::treasure::treasure_class& cls)
// {
// 	uint32_t treasureUid = static_cast<uint32_t>(m_treasures.size());
// 	m_treasures[treasureUid] = treasure(treasureUid, cls);
// 	treasure& t = m_treasures[treasureUid];

// 	auto templ = templates::metadata::get_instance().get_treasure_template(cls);
// 	for (auto comp : templ.components())
// 	{
// 		std::random_device rd;
// 		std::mt19937 generator(rd());
// 		std::uniform_real_distribution<double> distribution(0.0, 100.0);

// 		if (distribution(generator) <= static_cast<double>(templ[comp].probability))
// 		{
// 			if (comp == defs::treasure::component::copper)
// 			{
// 				t += create_amount(defs::coin::copper, templ[comp].countFrom, templ[comp].countTo);
// 			}
// 			else if (comp == defs::treasure::component::silver)
// 			{
// 				t += create_amount(defs::coin::silver, templ[comp].countFrom, templ[comp].countTo);
// 			}
// 			else if (comp == defs::treasure::component::gold)
// 			{
// 				t += create_amount(defs::coin::gold, templ[comp].countFrom, templ[comp].countTo);
// 			}
// 			else if (comp == defs::treasure::component::platinum_or_electrum)
// 			{
// 				std::random_device rd;
// 				std::mt19937 generator(rd());
// 				std::uniform_int_distribution<int> distribution(1, 100);

// 				if (distribution(generator) % 2 == 0)
// 					t += create_amount(defs::coin::platinum, templ[comp].countFrom, templ[comp].countTo);
// 				else
// 					t += create_amount(defs::coin::electrum, templ[comp].countFrom, templ[comp].countTo);
// 			}
// 			else if (comp == defs::treasure::component::gems)
// 			{
// 				std::random_device rd;
// 				std::mt19937 generator(rd());
// 				std::uniform_int_distribution<uint32_t> distribution(templ[comp].countFrom, templ[comp].countTo);
// 				uint32_t count = distribution(generator);

// 				for (uint32_t cnt = 0; cnt < count; ++cnt)
// 				{
// 					auto gemID = pick_gem();
// 					auto gt = templates::metadata::get_instance().get_gem_template(gemID);

// 					money::amount gemValue = create_amount(gt.get_value_currency(), gt.get_value_from(), gt.get_value_to());
// 					gem g(m_lastItemId++, gemID, gemValue);

// 					t += g;
// 				}
// 			}
// 			else if (comp == defs::treasure::component::art_objects)
// 			{
// 				std::random_device rd;
// 				std::mt19937 generator(rd());
// 				std::uniform_int_distribution<uint32_t> distribution(templ[comp].countFrom, templ[comp].countTo);
// 				uint32_t count = distribution(generator);

// 				for (uint32_t cnt = 0; cnt < count; ++cnt)
// 				{
// 					auto d = random::die(defs::die_faces::ten);
// 					auto prob = (d.roll<short>() - 1) * 10 + (d.roll<short>() - 1);

// 					auto objInfo = m_objectsOfArtProbability.lower_bound(prob)->second;

// 					money::amount objValue = create_amount(std::get<2>(objInfo), std::get<3>(objInfo), std::get<4>(objInfo));
// 					object_of_art o(m_lastItemId++, objValue);
				
// 					t += o;
// 				}
// 			}
// 			else if (comp == defs::treasure::component::magical_item
// 					|| comp == defs::treasure::component::additional_magical_item)
// 			{
// 				std::random_device rd;
// 				std::mt19937 generator(rd());
// 				std::uniform_int_distribution<uint32_t> distribution(templ[comp].countFrom, templ[comp].countTo);
// 				uint32_t count = distribution(generator);

// 				for (uint32_t cnt = 0; cnt < count; ++cnt)
// 				{
// 					auto magicalID = pick_magical();
// 					auto mt = templates::metadata::get_instance().get_magical_item_template(magicalID);

// 					uint32_t uid = equipment::items_pool::get_instance().new_uid();

// 					equipment::magical_item magItem(uid, mt.id(), mt.description(), mt.item_class(), mt.item_type(), mt.weight(),
// 												mt.hand_required(), mt.unidentified_name(), mt.magical_id(), mt.attack_type(),
// 												mt.weapon_group(), mt.body_slot());
// 					t += magItem;
// 				}
// 			}
// 		}
// 	}

// 	return t;
// }