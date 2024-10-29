#include <store.h>
#include <dice.h>
#include <options.h>
#include <cyclopedia.h>
#include <tables.h>

#include <cmath>
#include <optional>

Adndtk::Store::Store()
    : _name{}
{
    init();
}

Adndtk::Store::Store(const std::string& name)
    : _name{name}
{
    init();
}

Adndtk::Store::~Store()
{
}

void Adndtk::Store::init()
{
    auto cpMin = OptionalRules::get_instance().option<uint32_t>(Option::store_min_cp_starting_amount);
    auto cpMax = OptionalRules::get_instance().option<uint32_t>(Option::store_max_cp_starting_amount);
    auto spMin = OptionalRules::get_instance().option<uint32_t>(Option::store_min_sp_starting_amount);
    auto spMax = OptionalRules::get_instance().option<uint32_t>(Option::store_max_sp_starting_amount);
    auto epMin = OptionalRules::get_instance().option<uint32_t>(Option::store_min_ep_starting_amount);
    auto epMax = OptionalRules::get_instance().option<uint32_t>(Option::store_max_ep_starting_amount);
    auto gpMin = OptionalRules::get_instance().option<uint32_t>(Option::store_min_gp_starting_amount);
    auto gpMax = OptionalRules::get_instance().option<uint32_t>(Option::store_max_gp_starting_amount);
    auto ppMin = OptionalRules::get_instance().option<uint32_t>(Option::store_min_pp_starting_amount);
    auto ppMax = OptionalRules::get_instance().option<uint32_t>(Option::store_max_pp_starting_amount);

    _money.add(Defs::coin::copper_piece, Die::roll(cpMin, cpMax));
    _money.add(Defs::coin::silver_piece, Die::roll(spMin, spMax));
    _money.add(Defs::coin::electrum_piece, Die::roll(epMin, epMax));
    _money.add(Defs::coin::gold_piece, Die::roll(gpMin, gpMax));
    _money.add(Defs::coin::platinum_piece, Die::roll(ppMin, ppMax));
}

void Adndtk::Store::supply(const Defs::equipment& equipmentId, const short& count/*=1*/)
{
    if (_goods.find(equipmentId) == _goods.end())
    {
        _goods[equipmentId] = count;
    }
    else
    {
        _goods[equipmentId] += count;
    }
}

void Adndtk::Store::supply_all(const short& count/*=1*/)
{
    auto equipmentIds = Tables::equipment::to_vector<Defs::equipment>("id");
    for (auto& id : equipmentIds)
    {
        supply(id, count);
    }
}

void Adndtk::Store::remove(const Defs::equipment& equipmentId, const short& count/*=1*/)
{
    if (!check_supply(equipmentId, count))
    {
        throw std::runtime_error("The specified quantity is not available");
    }
    _goods[equipmentId] -= count;
}

bool Adndtk::Store::check_availability(const Defs::coin& currency, const uint32_t& amount, bool strictCheck/*=false*/) const
{
    if (OptionalRules::get_instance().option<bool>(Option::unlimited_store_availability))
    {
        return true;
    }

    bool available{false};
    if (strictCheck)
    {
        auto& currAmt = _money[currency];
        available = currAmt >= amount;
    }
    else
    {
        available = _money.check_availability(currency, amount);
    }

    return available;
}

bool Adndtk::Store::check_supply(const Defs::equipment& goodsId, const int& count) const
{
    if (OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply))
    {
        return true;
    }

    return (_goods.find(goodsId) != _goods.end() && _goods.at(goodsId) >= count);
}

double Adndtk::Store::get_amount(const double& minPrice, const std::optional<double>& maxPrice) const
{
    if (!maxPrice.has_value())
    {
        return minPrice;
    }
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(minPrice, maxPrice.value());
    auto prc = distribution(generator);

    return std::round(prc);
}

std::tuple<Adndtk::Defs::coin, double, std::optional<double>> Adndtk::Store::get_price_min_max(const Defs::equipment& id) const
{
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_equipment, id);
    const QueryResult& res = rs[0];

    auto currency = static_cast<Defs::coin>(res.as<short>("cost_coin"));
    auto minCost = res.as<double>("cost_min");
    auto maxCost = res.try_as<double>("cost_max");

    return std::make_tuple(currency, minCost, maxCost);
}

Adndtk::Coin Adndtk::Store::get_price(const Defs::equipment& id)
{
    auto [currency, minCost, maxCost] = get_price_min_max(id);

    auto amount{0};
    if (maxCost.has_value())
    {
        if (_variablePrices.find(id) == _variablePrices.end())
        {
            amount = get_amount(minCost, maxCost);
            _variablePrices[id] = Coin(currency, amount);
        }
        amount = _variablePrices[id].value();
    }
    else
    {
        amount = get_amount(minCost, maxCost);
    }

    Coin price(currency, static_cast<uint32_t>(amount));
    return std::move(price);
}

Adndtk::Coin Adndtk::Store::get_buy_price(const Defs::equipment& id, const short& count/*=1*/)
{
    auto price = get_price(id);
    auto profitPerc = OptionalRules::get_instance().option<double>(Option::store_profit_margin_percentage);
    double profit = ceil(price.value() * profitPerc);

    auto [currency, minCost, maxCost] = get_price_min_max(id);
    Coin cMin(currency, minCost);

    auto unitaryPrice = price.value() - profit;
    if (cMin > unitaryPrice)
    {
        unitaryPrice = minCost;
    }
    return std::move(Coin(price.currency(), unitaryPrice * count));
}

Adndtk::Coin Adndtk::Store::get_sell_price(const Defs::equipment& id, const short& count/*=1*/)
{
    auto price = get_price(id);
    auto profitPerc = OptionalRules::get_instance().option<double>(Option::store_profit_margin_percentage);
    double profit = ceil(price.value() * profitPerc);

    auto [currency, minCost, maxCost] = get_price_min_max(id);

    if (!maxCost.has_value())
    {
        maxCost = minCost;
    }
    
    Coin cMax(currency, maxCost.value());

    auto unitaryPrice = price.value() + profit;
    if (cMax < unitaryPrice && maxCost.has_value())
    {
        unitaryPrice = maxCost.value();
    }
    return std::move(Coin(price.currency(), unitaryPrice * count));
}