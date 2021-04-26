#pragma once

#include <string>

class Skill {
  friend std::ostream& operator<<(std::ostream&, const Skill&);
 public:
  Skill(std::string name, int level): name(name), level(level) {}

 private:
  std::string name;
  int         level;
};


std::ostream& operator<<(std::ostream& os, const Skill& skill) {
  os << skill.name + std::to_string(skill.level);
  return os;
}
