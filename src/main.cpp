#include <string>
#include <map>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/utils/filesystem.hpp>

#include "../lib/charm.hpp"
#include "../lib/logger.hpp"
#include "../lib/util.hpp"


cv::Mat diffAtPoint(
  const cv::Mat& image,
  const cv::Mat& templateImage,
  const cv::Point& point,
  const std::optional<cv::Size>& size,
  const int maskBinaryThreshold,
  const int diffBinaryThreshold
) {
  const auto _size = size.has_value() ? *size : cv::Size(templateImage.cols, templateImage.rows);
  const cv::Mat trimmed = image(cv::Rect(point, _size));

  cv::Mat templateMask;
  cv::threshold(templateImage, templateMask, maskBinaryThreshold, 255, cv::THRESH_BINARY);

  cv::Mat masked;
  trimmed.copyTo(masked, templateMask);

  cv::Mat diff;
  cv::absdiff(templateImage, masked, diff);
  cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);

  cv::Mat result;
  cv::threshold(diff, result, diffBinaryThreshold, 255, cv::THRESH_BINARY);
  // cv::imwrite("./tmp/debug.png", diff); // for debug

  return result;
}


template <typename T>
std::pair<T, cv::Mat> getMostMatchedImage(
  const cv::Mat& image,
  const std::map<T, cv::Mat>& templates,
  const cv::Point& point,
  const int maskBinaryThreshold = 63,
  const int diffBinaryThreshold = 63
) {
  int minDiffCount = INT_MAX;
  const std::pair<const T, cv::Mat> *candidate = nullptr;

  for (const auto& pair: templates) {
    const int diffCount = cv::countNonZero(diffAtPoint(image, pair.second, point, std::nullopt, maskBinaryThreshold, diffBinaryThreshold));
    if ( minDiffCount > diffCount ) {
      minDiffCount = diffCount;
      candidate = &pair;
    }
  }

  // logger.debug() << "minDiffCount: " << minDiffCount << std::endl;
  return *candidate;
}


int getCurrentPage(const cv::Mat& screenshot) {
  const auto POINT_PAGE = cv::Point(
    screenshot.cols * 787 / 1280,
    screenshot.rows * 582 / 720
  );

  static const auto dirpath = "templates/pages/";
  static const std::vector<cv::String> filepaths = listFiles(dirpath, "*.png");
  static const std::map<std::string, cv::Mat> templateImages = loadImages(filepaths, dirpath);

  const auto matched = getMostMatchedImage(screenshot, templateImages, POINT_PAGE, 31, 63).first;
  return std::stoi(matched);
}


std::pair<std::pair<int, int>, double> getCurrentCharmPos(const cv::Mat& screenshot) {
  const auto POINT_CHARM_AREA_LEFT_TOP = cv::Point(635, 359);
  const auto SIZE_CHARM_AREA = cv::Size(356, 199);

  const cv::Mat trimmed = screenshot(cv::Rect(POINT_CHARM_AREA_LEFT_TOP, SIZE_CHARM_AREA));

  static const auto pathTemplate = "templates/frame.png";
  static const auto templateImage = cv::imread(pathTemplate, cv::IMREAD_COLOR);

  cv::Mat result;
  cv::matchTemplate(trimmed, templateImage, result, cv::TM_CCOEFF_NORMED);

  cv::Point maxPoint;
  double maxValue;
  cv::minMaxLoc(result, NULL, &maxValue, NULL, &maxPoint);

  // logger.debug() << "match: " << maxValue << std::endl;

  return std::make_pair(
    std::make_pair(
      1 + int(0.5 + maxPoint.x/36.0),
      1 + int(0.5 + maxPoint.y/41.0)
    ),
    maxValue
  );
}


int getRarity(const cv::Mat& screenshot) {
  const auto POINT_RARITY = cv::Point(
    screenshot.cols * 1190 / 1280,
    screenshot.rows * 176  / 720
  );

  static const auto dirpath = "submodules/SZ7M8ci7/MHrise/template/rare/";
  static const std::vector<cv::String> filepaths = listFiles(dirpath, "");
  static const std::map<std::string, cv::Mat> templateImages = loadImages(filepaths, dirpath);

  const auto matched = getMostMatchedImage(screenshot, templateImages, POINT_RARITY, 63, 63).first;
  return std::stoi(matched);
}


int getSlots(const cv::Mat& screenshot) {
  const auto POINT_SLOTS = cv::Point(
    screenshot.cols * 1160 / 1280,
    screenshot.rows * 200  / 720
  );

  static const auto dirpath = "submodules/SZ7M8ci7/MHrise/template/slot/";
  static const std::vector<cv::String> filepaths = listFiles(dirpath, "");
  static const std::map<std::string, cv::Mat> templateImages = loadImages(filepaths, dirpath);

  const auto matched = getMostMatchedImage(screenshot, templateImages, POINT_SLOTS, 0, 63).first;
  return std::stoi(matched);
}


std::vector<std::string> getSkills(const cv::Mat& screenshot) {
  const auto POINTS_SKILL = std::vector<cv::Point>{
    {
      screenshot.cols * 1033 / 1280,
      screenshot.rows * 266  / 720
    },
    {
      screenshot.cols * 1033 / 1280,
      screenshot.rows * 317  / 720
    },

  };

  static const auto dirpath = "submodules/SZ7M8ci7/MHrise/template/skill/";
  static const std::vector<cv::String> filepaths = listFiles(dirpath, "");
  static const std::map<std::string, cv::Mat> templateImages = loadImages(filepaths, dirpath);

  const std::vector<std::string> skills = {
    getMostMatchedImage(screenshot, templateImages, POINTS_SKILL[0], 0, 63).first,
    getMostMatchedImage(screenshot, templateImages, POINTS_SKILL[1], 0, 63).first,
  };

  return skills;
}


std::vector<int> getSkillLevels(const cv::Mat& screenshot) {
  const auto POINTS_SKILL_LEVEL = std::vector<cv::Point>{
    {
      screenshot.cols * 1190 / 1280,
      screenshot.rows * 289  / 720
    },
    {
      screenshot.cols * 1190 / 1280,
      screenshot.rows * 340  / 720
    },

  };

  static const auto dirpath = "submodules/SZ7M8ci7/MHrise/template/lvl/";
  static const std::vector<cv::String> filepaths = listFiles(dirpath, "");
  static const std::map<std::string, cv::Mat> templateImages = loadImages(filepaths, dirpath);

  const std::vector<int> skillLevels = {
    stoi(getMostMatchedImage(screenshot, templateImages, POINTS_SKILL_LEVEL[0], 0, 63).first),
    stoi(getMostMatchedImage(screenshot, templateImages, POINTS_SKILL_LEVEL[1], 0, 63).first),
  };

  return skillLevels;
}


Charm scanCharmScreenshot(const cv::Mat& screenshot) {
  const int rarity = getRarity(screenshot);
  int slots = getSlots(screenshot);
  const auto skills = getSkills(screenshot);
  const auto skillLevels = getSkillLevels(screenshot);

  // logger.debug() << "rarity: " << rarity << std::endl;
  // logger.debug() << "slots: " << slots << std::endl;

  // for (const auto& skill: skills) {
  //   logger.debug() << "skill: " << skill << std::endl;
  // }

  // for (const auto& skillLevel: skillLevels) {
  //   logger.debug() << "skill level: " << skillLevel << std::endl;
  // }

  std::vector<CharmSlot> vecSlots = {};
  for (int i = 0; slots != 0 && i < 3; i++) {
    vecSlots.push_back( CharmSlot(slots % 10) );
    slots /= 10;
  }
  std::reverse(std::begin(vecSlots), std::end(vecSlots));

  return Charm(
    "護石",
    rarity,
    vecSlots,
    { Skill(skills[0], skillLevels[0]), Skill(skills[1], skillLevels[1]) }
  );
}


int main(void) {
  std::string inputDir = "input/";

  for (const auto& filename: listFiles(inputDir, "*.mp4")) {
    logger.trace() << filename << std::endl;
    cv::VideoCapture video;
    video.open(inputDir + filename);

    if (video.isOpened() == false) {
      logger.error() << "failed to open " << inputDir << filename << std::endl;
      exit(1);
    }

    std::map<int, Charm> charms = {};
    cv::Mat image;
    while (1) {
      video >> image;
      if (image.empty() == true) break;

      const auto [pos, match] = getCurrentCharmPos(image);
      if ( match < 0.4 ) {
        // 放置すると blink するので一致度が低い時はスキップ
        // logger.debug() << "less match degress "
        //                << match
        //                << " for charm position seeking. skip"
        //                << std::endl;
        continue;
      }
      const int page = getCurrentPage(image);

      const int ITEMS_PER_PAGE = 50;
      const int ITEMS_PER_ROW  = 10;
      // assign unique ID from its position (page, row and col)
      const int id = (ITEMS_PER_PAGE * (page - 1)) + (ITEMS_PER_ROW * (pos.second - 1)) + pos.first;
      if ( ! charms.count(id) ) { // is not defined
        charms.insert(std::make_pair(id, scanCharmScreenshot(image)));
        // logger.debug() << "match: " << match << std::endl;
        logger.debug() << "pos: " << 10 * (pos.second - 1) + pos.first << std::endl;
        logger.debug() << "page: " << page << std::endl;

        logger.info() << charms[id] << std::endl;
      }
      else {
        // logger.debug() << "the charm ID" << id << " is already scanned. skip" << std::endl;
      }

      // cv::imwrite("tmp/debug.png", image);
    }
  }

  return 0;
}
