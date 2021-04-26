#include <vector>
#include <map>
#include <sstream>
#include <iterator>
#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/img_hash.hpp>


std::string join(std::vector<std::string> strings, std::string delimiter = ", ") {
  std::ostringstream os;
  std::copy(std::begin(strings), std::end(strings), std::ostream_iterator<std::string>(os, delimiter.c_str()));
  std::string s = os.str();

  s.erase(s.size() - delimiter.length());
  return s;
}


std::vector<cv::String> listFiles(const std::string& dir, const std::string& filter = "") {
  std::vector<cv::String> filepaths;
  cv::utils::fs::glob_relative(dir, filter, filepaths);

  return filepaths;
}


std::map<std::string, cv::Mat> loadImages(const std::vector<cv::String>& filepaths, const std::string& dirpath = "") {
  std::map<std::string, cv::Mat> buf;

  for (const auto& filepath: filepaths) {
    const auto pos = filepath.find_last_of(".");
    const auto basename = (pos == std::string::npos) ? filepath : filepath.substr(0, pos);
    buf.insert(std::make_pair(basename, cv::imread(dirpath + filepath, cv::IMREAD_COLOR)));
  }

  return buf;
}


cv::Mat phash(const cv::Mat& matrix) {
  const auto phash = cv::img_hash::PHash::create();

  cv::Mat hash;
  phash->compute(matrix, hash);

  return hash;
}


std::string mat2str(const cv::Mat& matrix) {
  std::stringstream ss;
  ss << matrix;
  return ss.str();
}


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
