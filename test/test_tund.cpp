//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-TUND.001] Turning undead (skeleton/1HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::skeleton, Defs::turnable::hd1})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.002] Turning undead (zombie) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::zombie})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.003] Turning undead (ghoul/2HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghoul, Defs::turnable::hd2})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.004] Turning undead (shadow/3-4HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::shadow, Defs::turnable::hd3, Defs::turnable::hd4})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.005] Turning undead (wight/5HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::wight, Defs::turnable::hd5})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.006] Turning undead (ghast) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghast})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.007] Turning undead (wraith/6HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::wraith, Defs::turnable::hd6})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.008] Turning undead (mummy/7HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::mummy, Defs::turnable::hd7})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.009] Turning undead (spectre/8HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::spectre, Defs::turnable::hd8})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.010] Turning undead (vampire/9HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::vampire, Defs::turnable::hd9})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);
    }
}

TEST_CASE("[TC-TUND.011] Turning undead (ghost/10HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghost, Defs::turnable::hd10})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);
    }
}

TEST_CASE("[TC-TUND.012] Turning undead (ghost/10HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::lich, Defs::turnable::hd11_or_more})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);
    }
}

TEST_CASE("[TC-TUND.013] Turning undead (special) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::special})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);
    }
}