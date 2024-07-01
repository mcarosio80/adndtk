#ifndef TREASURE_H
#define TREASURE_H

#include <defs.h>
#include <coin.h>
#include <common_types.h>
#include <money_bag.h>
#include <dice.h>

#include <cstdint>
#include <optional>
#include <string>
#include <random>
#include <utility>
#include <set>

namespace Adndtk
{
    class TreasurePool
    {
    public:
        static TreasurePool& get_instance();
        TreasurePool(TreasurePool const&) = delete;
        void operator=(TreasurePool const&) = delete;

        //const treasure& create(const defs::treasure::treasure_class& cls);
        //void detatch(const uint32_t& uid);

        Coin generate_value(const Defs::coin& coin, const uint32_t& valueFrom, const uint32_t& valueTo);
        Coin generate_value(const std::optional<Defs::coin>& rangeCoin, const std::optional<uint32_t>& valueFrom, const std::optional<uint32_t>& valueTo, const Defs::coin& baseCoin, const uint32_t& baseValue);
        uint32_t get_next_id() { return std::exchange(_lastItemId, _lastItemId+1); }

        Defs::gem pick_gem();
        Defs::magical_item pick_magical();

    private:
        TreasurePool();

        static bool                             _initialised;
        uint32_t						        _lastItemId;
        std::random_device                      _rd;
        std::mt19937                            _generator;
        std::uniform_int_distribution<uint32_t> _uniformDistribution;
        static const uint32_t                   _valuesFrom{1};
        static const uint32_t                   _valuesTo{10000};

        Defs::magical_item pick_potion();
        Defs::magical_item pick_scroll();
        Defs::magical_item pick_ring();
        Defs::magical_item pick_rod();
        Defs::magical_item pick_stave();
        Defs::magical_item pick_wand();
        Defs::magical_item pick_book();
        Defs::magical_item pick_jewel();
        Defs::magical_item pick_cloak();
        Defs::magical_item pick_boot();
        Defs::magical_item pick_girdle();
        Defs::magical_item pick_bag();
        Defs::magical_item pick_dust();
        Defs::magical_item pick_household();
        Defs::magical_item pick_musical();
        Defs::magical_item pick_wierd();
        Defs::magical_item pick_armour();
        Defs::magical_item pick_weapon();
    };

    class Gem
    {
    public:
        Gem();
        Gem(const Defs::gem& gemId);
        ~Gem() {}

        const uint32_t& uid() const { return _uid; }
        const Defs::gem& id() const { return _gemId; }
        const std::string& name() const { return _name; }
        const Defs::gem_type& type() const { return _gemType; }
        const Coin& value() const { return _value; }
    
    protected:
        uint32_t		_uid;
        Defs::gem       _gemId;
        std::string     _name;
        Defs::gem_type  _gemType;
        Coin            _value;
    };

    class ObjectOfArt
    {
    public:
        ObjectOfArt(const std::optional<std::string> name = std::nullopt);
        ~ObjectOfArt();

        const short& object_type() const { return _objectType; }
        const uint32_t& uid() const { return _uid; }
        const std::string& name() const { return _name; }
        const Coin& value() const { return _value; }

    protected:
        short           _objectType;
        uint32_t		_uid;
        std::string     _name;
        Coin            _value;
    };

    class MagicalItem
    {
    public:
        MagicalItem();
        MagicalItem(const Defs::magical_item& itemId);
        ~MagicalItem();

        void identify();

        const auto& id() const { return _id; }
        const auto& uid() const { return _uid; }
        const auto& name() const;
        auto nature() const;
        const auto& type() const { return _type; }
        const auto& xp_value() const { return _xpValue; }
        const auto& equipment_type() const { return _itemType; }
        const auto& equipment_id() const { return _equipmentId; }

    protected:
        Defs::magical_item                          _id;
        uint32_t		                            _uid;
        std::string                                 _name;
        std::string                                 _unidentifiedName;
        Defs::magical_item_nature                   _nature;
        Defs::magical_item_type                     _type;
        std::optional<XP>                           _xpValue;
        Defs::equipment_type                        _itemType;
        std::optional<Defs::equipment>              _equipmentId;
        bool                                        _identified;

        std::set<Defs::character_class>             _allowedClasses;
        std::set<Defs::character_class_type>        _allowedClassTypes;
        std::set<Defs::moral_alignment>             _allowedAlignments;
    };

    class Treasure
    {
    public:
        Treasure(const Defs::treasure_class& cls);
        ~Treasure();

        const uint32_t& uid() { return _uid; }

        Treasure& operator+= (const Adndtk::Coin& amt);
        Treasure& operator+= (const Gem& g);
        Treasure& operator+= (const ObjectOfArt& o);
        Treasure& operator+= (const MagicalItem& m);

        std::set<Defs::coin>			coins();
        Adndtk::Coin		            value(const Defs::coin& currency);
        const std::vector<Gem>&			gems();
        const std::vector<ObjectOfArt>&	objects_of_art();
        const std::vector<MagicalItem>&	magical_items();

        double							total_value(const Defs::coin& currency);

    private:
        uint32_t								_uid;
        Defs::treasure_class			        _class;
        Adndtk::MoneyBag    					_coins;
        std::vector<Gem>						_gems;
        std::vector<ObjectOfArt>				_objectsOfArt;
        std::vector<MagicalItem>		        _magicalItems;

        template <typename _T>
        _T select_one(const std::vector<_T>& v)
        {
            auto r = Die::roll(1, v.size());
			auto it = std::next(v.begin(), r);
			return *it;
        }

        void add_coins(const Defs::coin& coin, const uint32_t& countFrom, const uint32_t& countTo);
        void add_gem(const uint32_t& countFrom, const uint32_t& countTo);
        void add_object_of_art(const uint32_t& countFrom, const uint32_t& countTo);
        void add_magical_item(const uint32_t& countFrom, const uint32_t& countTo, const int& nature, const std::optional<int>& additionalComponent, const std::optional<int>& additionalCount);
        std::vector<Defs::magical_item_type> get_magical_types(const int& nature);
    };
}

#endif // TREASURE_H