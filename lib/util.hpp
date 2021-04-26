#pragma once

#include <vector>
#include <sstream>
#include <iterator>
#include <opencv2/core.hpp>


std::string join(std::vector<std::string> strings, std::string delimiter = ", ");
std::vector<cv::String> listFiles(const std::string& dir, const std::string& filter = "");
std::map<std::string, cv::Mat> loadImages(const std::vector<cv::String>& filepaths, const std::string& dirpath = "");
cv::Mat phash(const cv::Mat& matrix);
std::string mat2str(const cv::Mat& matrix);

class StringJoin {
 public:
  StringJoin(const char* sep) :sep(sep) {};

  template <typename T>
  StringJoin& operator<<(const T& v) {
    if ( isEmpty ) {
      buf << v;
      isEmpty = false;
    }
    else {
      buf << sep << v;
    }
    return *this;
  }

  friend
  std::ostream& operator<<(std::ostream& os, const StringJoin& s) {
    return os << s.buf.str();
  }

 private:
  const char* sep;
  std::ostringstream buf;
  bool isEmpty = true;
};
