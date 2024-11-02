#include <saving_throws.h>
#include <cyclopedia.h>
#include <dice.h>


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

Adndtk::SavingThrows::SavingThrows()
{
}

Adndtk::SavingThrows::~SavingThrows()
{
}

void Adndtk::SavingThrows::init()
{
    auto result = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_saving_throws);

    for (auto& t : result)
    {
        auto cType = t.as<Defs::character_class_type>("class_type_id");
        auto level = t.as<ExperienceLevel>("level");
        auto savId = t.as<Defs::saving_throw>("saving_throw_id");
        auto score = t.as<SavingScore>("score");

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

const Adndtk::SavingScore& Adndtk::SavingThrows::get_score(const Defs::character_class& cls, const CharacterExperience& levels, const Defs::saving_throw& sav) const
{
    auto score = save_as(cls, levels, sav);
    return get_score(score.first, score.second, sav);
}
        
bool Adndtk::SavingThrows::roll(const Defs::character_class_type& type, const ExperienceLevel& lvl, const Defs::saving_throw& sav, const short& bonusMalus/*=0*/) const
{
    Die d20{Defs::die::d20};
    auto roll = d20;
    auto target = get_score(type, lvl, sav);

    return roll + bonusMalus >= target;
}

bool Adndtk::SavingThrows::roll(const Defs::character_class& clsId, const CharacterExperience& levels, const Defs::saving_throw& sav, const short& bonusMalus) const
{
    auto saveScore = save_as(clsId, levels, sav);
    return roll(saveScore.first, saveScore.second, sav, bonusMalus);
}

std::pair<Adndtk::Defs::character_class_type, Adndtk::ExperienceLevel> Adndtk::SavingThrows::save_as(const Defs::character_class& clsId, const CharacterExperience& levels, const Defs::saving_throw& sav) const
{
    auto classes =  Cyclopedia::get_instance().split<Defs::character_class>(clsId);
    
    SavingScore bestSave = Const::base_saving_throw;
    Defs::character_class_type bestClsType = Defs::character_class_type::warrior;
    ExperienceLevel bestLvl = 0;
    for (auto cls : classes)
    {
        auto clsTyp = Cyclopedia::get_instance().get_class_type(cls);
        auto clsLvl = levels.level(cls);
        auto score = get_score(clsTyp, clsLvl, sav);
        if (score < bestSave)
        {
            bestClsType = clsTyp;
            bestLvl = clsLvl;
            bestSave = score;
        }
    }
    
    return std::make_pair(bestClsType, bestLvl);
}