#pragma once

#include <vector>
#include "charm-slot.hpp"
#include "skill.hpp"
#include "util.hpp"


class Charm {
  friend std::ostream& operator<<(std::ostream&, const Charm&);

 public:
  Charm(): name("unnamed"), rarity(-1), slots(-1), skills() {}
  Charm(
    std::string name,
    int rarity,
    std::vector<CharmSlot> slots,
    std::vector<Skill> skills
  ): name(name), rarity(rarity), slots(slots), skills(skills) {
    for (int i = 0; i < 3 - static_cast<int>(slots.size()); i++) {
      this->slots.push_back(CharmSlot());
    }
  };


 private:
  std::string             name;
  int                     rarity;

  std::vector<CharmSlot>  slots;
  std::vector<Skill>      skills;
};


std::ostream& operator<<(std::ostream& os, const Charm& charm) {
  os << charm.name + " " + std::to_string(charm.rarity) + " ";

  StringJoin slots("-");
  for ( const auto& slot: charm.slots ) { slots << slot; }

  StringJoin skills(",");
  for ( const auto& skill: charm.skills ) { skills << skill; }

  os << slots << " " << skills;

  return os;
}
