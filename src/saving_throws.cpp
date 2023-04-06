#include <saving_throws.h>
#include <cyclopedia.h>

bool Adndtk::SavingThrows::_initialised = false;


Adndtk::SavingThrows& Adndtk::SavingThrows::get_instance()
{
    static SavingThrows _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

Adndtk::SavingThrows::~SavingThrows()
{
}

void Adndtk::SavingThrows::init()
{
    auto result = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_saving_throws);

    for (auto& t : result)
    {
        auto cType = static_cast<Defs::character_class_type>(t.as<int>("class_type_id"));
        auto level = static_cast<ExperienceLevel>(t.as<int>("level"));
        auto savId = static_cast<Defs::saving_throw>(t.as<int>("saving_throw_id"));
        auto score = static_cast<SavingScore>(t.as<int>("score"));

        _savingScores[cType][level][savId] = score;
    }
}

const Adndtk::SavingScore& Adndtk::SavingThrows::get_score(const Adndtk::Defs::character_class_type& type, const Adndtk::ExperienceLevel& lvl, const Adndtk::Defs::saving_throw& sav) const
{
    if (lvl <= 0)
    {
        return _savingScores.at(Defs::character_class_type::warrior).at(0).at(sav);
    }

    auto& map = _savingScores.at(type);
    if (map.find(lvl) == map.end())
    {
        auto t = map.lower_bound(lvl);
        return t->second.at(sav);
    }
    
    return map.at(lvl).at(sav);
}