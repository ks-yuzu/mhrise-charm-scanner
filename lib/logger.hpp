#pragma once

#include <iostream>
#include <string>
#include <memory>

class Logger {
 public:
  Logger(std::ostream& os = std::cout): os(os) {};

  static const Logger& getDefaultLogger() {
    if ( !Logger::defaultLogger ) { Logger::defaultLogger = std::make_unique<Logger>(); }
    return *(Logger::defaultLogger);
  }

  std::ostream& error() {
    os << "[error] ";
    return os;
  }

  std::ostream& info() {
    os << "[info] ";
    return os;
  }

  std::ostream& debug() {
    os << "[debug] ";
    return os;
  }

  std::ostream& trace() {
    os << "[trace] ";
    return os;
  }
// void warn() { print("[warn] " + std::string(msg)); }
  // void error() { print("[error] " + std::string(msg)); }


 private:
  void print(std::string_view msg) {
    this->os << msg << std::endl;
  }

  inline static std::unique_ptr<Logger> defaultLogger;

  std::ostream &os;
};

extern Logger logger;
