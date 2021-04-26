#pragma once

#include <string>
#include "../lib/logger.hpp"

class CharmSlot {
  friend std::ostream& operator<<(std::ostream&, const CharmSlot&);

 public:
  CharmSlot(int level = 0): level(level) {};

 private:
  int level;
};



std::ostream& operator<<(std::ostream& os, const CharmSlot& charmSlot) {
  os << charmSlot.level;
  return os;
}

