#include <options.h>

bool Adndtk::OptionalRules::_initialised = false;


Adndtk::OptionalRules& Adndtk::OptionalRules::get_instance()
{
    static OptionalRules _instance;

    if (!_initialised)
    {
        _initialised = true;
    }
    return _instance;
}

Adndtk::OptionalRules::OptionalRules()
{
}

Adndtk::OptionalRules::~OptionalRules()
{
}