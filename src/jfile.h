#pragma once

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <chrono>
using namespace std;
using namespace chrono;

#include <fmt/core.h>
#include <fmt/format.h>

#define source_info fmt::format("{}:{}", __FILE__, __LINE__)

#define logi(x, ...)                                                           \
  cout << fmt::format("{:<5} {:<25} ", "INFO", source_info)                    \
       << fmt::format(x, ##__VA_ARGS__) << endl

#define logd(x, ...)                                                           \
  cout << fmt::format("{:<5} {:<25} ", "DEBUG", source_info)                   \
       << fmt::format(x, ##__VA_ARGS__) << endl

#define loge(x, ...)                                                           \
  cout << fmt::format("{:<5} {:<25} ", "ERROR", source_info)                   \
       << fmt::format(x, ##__VA_ARGS__) << endl;                               \
  exit(-1)

#define fmt(x, ...) fmt::format(x, ##__VA_ARGS__)

class JFile {
public:
  JFile(const char *file_path);
  uint8_t *map();
  ssize_t write(uint8_t *data, size_t size);
  ssize_t write(const char *data);
  ssize_t write(string &str);
  vector<string> readlines();
  size_t size();
  bool is_exists();
  char *filePath();

public:
  ~JFile();

private:
  FILE *fd = nullptr;
  size_t file_size;
  char *file_path_;
  uint8_t *map_addr;
};

inline JFile::JFile(const char *filePath) {

  file_path_ = (char *)malloc(strlen(filePath) + 1);
  strcpy(file_path_, filePath);
  struct stat st;
  stat(file_path_, &st);
  file_size = st.st_size;
}

inline uint8_t *JFile::map() {
  if (!fd) {
    fd = fopen(file_path_, "rb");
    if (!fd) {
      printf("%s open failed! err: %s", file_path_, strerror(errno));
    }
  }
  map_addr = (uint8_t *)mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE, fileno(fd), 0);
  if (map_addr == MAP_FAILED) {
    printf("mmap failed! err: %s  file_path: %s", strerror(errno), file_path_);
  }
  return map_addr;
}

inline ssize_t JFile::write(uint8_t *data, size_t size) {
  if (!fd) {
    fd = fopen(file_path_, "wb");
    if (!fd) {
      printf("%s open failed! err: %s", file_path_, strerror(errno));
    }
  }
  return fwrite(data, 1, size, fd);
}

inline ssize_t JFile::write(const char *data) {
  if (!fd) {
    fd = fopen(file_path_, "wb");
    if (!fd) {
      printf("%s open failed! err: %s", file_path_, strerror(errno));
    }
  }
  return fwrite(data, 1, strlen(data), fd);
}

inline ssize_t JFile::write(string &str) {
  if (!fd) {
    fd = fopen(file_path_, "wb");
    if (!fd) {
      printf("%s open failed! err: %s", file_path_, strerror(errno));
    }
  }
  return fwrite(str.data(), 1, str.length(), fd);
}

inline vector<string> JFile::readlines() {
  vector<string> lines;
  ifstream file(file_path_);
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      lines.push_back(line);
    }
  }
  return lines;
}

inline size_t JFile::size() { return file_size; }

inline bool JFile::is_exists() { return (access(file_path_, 0) == 0); }

inline char *JFile::filePath() { return file_path_; }

inline JFile::~JFile() { free(file_path_); }
