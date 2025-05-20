#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <map>
#include <string>
#include <optional>

#include <battlefield.h>

namespace Adndtk
{
    class Battleground
    {
    public:
        static Battleground& get_instance();
        Battleground(Battleground const&) = delete;
        void operator=(Battleground const&) = delete;

        bool has_battlefield(const std::string& fieldName);
        void add_battlefield(const std::string& fieldName);
        Battlefield& get_battlefield(const std::string& fieldName);
        std::optional<Battlefield> try_get_battlefield(const std::string& fieldName) const;
        size_t size() const { return _fields.size(); };
        void clear();

    private:
        Battleground();
        ~Battleground();

		static bool                         _initialised;
        std::map<std::string, Battlefield>  _fields;
    };
}

#endif // BATTLEGROUND_H