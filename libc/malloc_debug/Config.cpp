/*
 * Copyright (C) 2015 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#include <string>
#include <vector>

#include <sys/system_properties.h>

#include <private/bionic_macros.h>

#include "Config.h"
#include "debug_log.h"

struct Feature {
  Feature(std::string name, size_t default_value, size_t min_value, size_t max_value,
          uint64_t option, size_t* value, bool* config, bool combo_option)
      : name(name), default_value(default_value), min_value(min_value), max_value(max_value),
        option(option), value(value), config(config), combo_option(combo_option) {}
  std::string name;
  size_t default_value = 0;
  size_t min_value = 0;
  size_t max_value = 0;

  uint64_t option = 0;
  size_t* value = nullptr;
  bool* config = nullptr;
  // If set to true, then all of the options following are set on until
  // for which the combo_option value is set.
  bool combo_option = false;
};

class PropertyParser {
 public:
  PropertyParser(const char* property) : cur_(property) {}

  bool Get(std::string* property, size_t* value, bool* value_set);

  bool Done() { return done_; }

  void LogUsage();

  static constexpr uint8_t DEFAULT_FILL_ALLOC_VALUE = 0xeb;
  static constexpr uint8_t DEFAULT_FILL_FREE_VALUE = 0xef;

  static constexpr uint8_t DEFAULT_FRONT_GUARD_VALUE = 0xaa;
  static constexpr uint8_t DEFAULT_REAR_GUARD_VALUE = 0xbb;

 private:
  const char* cur_ = nullptr;

  bool done_ = false;

  DISALLOW_COPY_AND_ASSIGN(PropertyParser);
};

bool PropertyParser::Get(std::string* property, size_t* value, bool* value_set) {
  // Process each property name we can find.
  while (isspace(*cur_))
    ++cur_;

  if (*cur_ == '\0') {
    done_ = true;
    return false;
  }

  const char* property_start = cur_;
  while (!isspace(*cur_) && *cur_ != '=' && *cur_ != '\0')
    ++cur_;

  *property = std::string(property_start, cur_ - property_start);

  // Skip any spaces after the name.
  while (isspace(*cur_) && *cur_ != '=' && *cur_ != '\0')
    ++cur_;

  if (*cur_ == '=') {
    ++cur_;
    errno = 0;
    *value_set = true;
    char* end;
    long read_value = strtol(cur_, const_cast<char**>(&end), 10);
    if (errno != 0) {
      error_log("%s: bad value for option '%s': %s", getprogname(), property->c_str(),
                strerror(errno));
      return false;
    }
    if (cur_ == end || (!isspace(*end) && *end != '\0')) {
      if (cur_ == end) {
        error_log("%s: bad value for option '%s'", getprogname(), property->c_str());
      } else {
        error_log("%s: bad value for option '%s', non space found after option: %s",
                  getprogname(), property->c_str(), end);
      }
      return false;
    } else if (read_value < 0) {
      error_log("%s: bad value for option '%s', value cannot be negative: %ld",
                getprogname(), property->c_str(), read_value);
      return false;
    }
    *value = static_cast<size_t>(read_value);
    cur_ = end;
  } else {
    *value_set = false;
  }
  return true;
}

void PropertyParser::LogUsage() {
  error_log("malloc debug options usage:");
  error_log("");
  error_log("  front_guard[=XX]");
  error_log("    Enables a front guard on all allocations. If XX is set");
  error_log("    it sets the number of bytes in the guard. The default is");
  error_log("    32 bytes.");
  error_log("");
  error_log("  rear_guard[=XX]");
  error_log("    Enables a rear guard on all allocations. If XX is set");
  error_log("    it sets the number of bytes in the guard. The default is");
  error_log("    32 bytes.");
  error_log("");
  error_log("  guard[=XX]");
  error_log("    Enables both a front guard and a rear guard on all allocations.");
  error_log("    If XX is set it sets the number of bytes in both guards.");
  error_log("    The default is 32 bytes.");
  error_log("");
  error_log("  backtrace[=XX]");
  error_log("    Enable capturing the backtrace at the point of allocation.");
  error_log("    If XX is set it sets the number of backtrace frames.");
  error_log("    The default is 16 frames.");
  error_log("");
  error_log("  backtrace_enable_on_signal[=XX]");
  error_log("    Enable capturing the backtrace at the point of allocation.");
  error_log("    The backtrace capture is not enabled until the process");
  error_log("    receives a signal. If XX is set it sets the number of backtrace");
  error_log("    frames. The default is 16 frames.");
  error_log("");
  error_log("  fill_on_alloc[=XX]");
  error_log("    On first allocation, fill with the value 0x%02x.", DEFAULT_FILL_ALLOC_VALUE);
  error_log("    If XX is set it will only fill up to XX bytes of the");
  error_log("    allocation. The default is to fill the entire allocation.");
  error_log("");
  error_log("  fill_on_free[=XX]");
  error_log("    On free, fill with the value 0x%02x. If XX is set it will",
            DEFAULT_FILL_FREE_VALUE);
  error_log("    only fill up to XX bytes of the allocation. The default is to");
  error_log("    fill the entire allocation.");
  error_log("");
  error_log("  fill[=XX]");
  error_log("    On both first allocation free, fill with the value 0x%02x on",
            DEFAULT_FILL_ALLOC_VALUE);
  error_log("    first allocation and the value 0x%02x. If XX is set, only fill",
            DEFAULT_FILL_FREE_VALUE);
  error_log("    up to XX bytes. The default is to fill the entire allocation.");
  error_log("");
  error_log("  expand_alloc[=XX]");
  error_log("    Allocate an extra number of bytes for every allocation call.");
  error_log("    If XX is set, that is the number of bytes to expand the");
  error_log("    allocation by. The default is 16 bytes.");
  error_log("");
  error_log("  free_track[=XX]");
  error_log("    When a pointer is freed, do not free the memory right away.");
  error_log("    Instead, keep XX of these allocations around and then verify");
  error_log("    that they have not been modified when the total number of freed");
  error_log("    allocations exceeds the XX amount. When the program terminates,");
  error_log("    the rest of these allocations are verified. When this option is");
  error_log("    enabled, it automatically records the backtrace at the time of the free.");
  error_log("    The default is to record 100 allocations.");
  error_log("");
  error_log("  free_track_backtrace_num_frames[=XX]");
  error_log("    This option only has meaning if free_track is set. This indicates");
  error_log("    how many backtrace frames to capture when an allocation is freed.");
  error_log("    If XX is set, that is the number of frames to capture. If XX");
  error_log("    is set to zero, then no backtrace will be captured.");
  error_log("    The default is to record 16 frames.");
  error_log("");
  error_log("  leak_track");
  error_log("    Enable the leak tracking of memory allocations.");
}

static bool SetFeature(
    const std::string name, const Feature& feature, size_t value, bool value_set) {
  if (feature.config) {
    *feature.config = true;
  }
  if (feature.value != nullptr) {
    if (value_set) {
      if (value < feature.min_value) {
        error_log("%s: bad value for option '%s', value must be >= %zu: %zu",
                  getprogname(), name.c_str(), feature.min_value, value);
        return false;
      } else if (value > feature.max_value) {
        error_log("%s: bad value for option '%s', value must be <= %zu: %zu",
                  getprogname(), name.c_str(), feature.max_value, value);
        return false;
      }
      *feature.value = value;
    } else {
      *feature.value = feature.default_value;
    }
  } else if (value_set) {
     error_log("%s: value set for option '%s' which does not take a value",
               getprogname(), name.c_str());
     return false;
  }
  return true;
}

// This function is designed to be called once. A second call will not
// reset all variables.
bool Config::SetFromProperties() {
  char property_str[PROP_VALUE_MAX];
  memset(property_str, 0, sizeof(property_str));
  if (!__system_property_get("libc.debug.malloc.options", property_str)) {
    return false;
  }

  // Initialize a few default values.
  fill_alloc_value = PropertyParser::DEFAULT_FILL_ALLOC_VALUE;
  fill_free_value = PropertyParser::DEFAULT_FILL_FREE_VALUE;
  front_guard_value = PropertyParser::DEFAULT_FRONT_GUARD_VALUE;
  rear_guard_value = PropertyParser::DEFAULT_REAR_GUARD_VALUE;
  backtrace_signal = SIGRTMIN + 10;
  free_track_backtrace_num_frames = 16;

  // Parse the options are of the format:
  //   option_name or option_name=XX

  // Supported features:
  const Feature features[] = {
    Feature("guard", 32, 1, 16384, 0, nullptr, nullptr, true),
    // Enable front guard. Value is the size of the guard.
    Feature("front_guard", 32, 1, 16384, FRONT_GUARD, &this->front_guard_bytes, nullptr, true),
    // Enable end guard. Value is the size of the guard.
    Feature("rear_guard", 32, 1, 16384, REAR_GUARD, &this->rear_guard_bytes, nullptr, true),

    // Enable logging the backtrace on allocation. Value is the total
    // number of frames to log.
    Feature("backtrace", 16, 1, 256, BACKTRACE | TRACK_ALLOCS, &this->backtrace_frames,
            &this->backtrace_enabled, false),
    // Enable gathering backtrace values on a signal.
    Feature("backtrace_enable_on_signal", 16, 1, 256, BACKTRACE | TRACK_ALLOCS,
            &this->backtrace_frames, &this->backtrace_enable_on_signal, false),

    Feature("fill", SIZE_MAX, 1, SIZE_MAX, 0, nullptr, nullptr, true),
    // Fill the allocation with an arbitrary pattern on allocation.
    // Value is the number of bytes of the allocation to fill
    // (default entire allocation).
    Feature("fill_on_alloc", SIZE_MAX, 1, SIZE_MAX, FILL_ON_ALLOC, &this->fill_on_alloc_bytes,
            nullptr, true),
    // Fill the allocation with an arbitrary pattern on free.
    // Value is the number of bytes of the allocation to fill
    // (default entire allocation).
    Feature("fill_on_free", SIZE_MAX, 1, SIZE_MAX, FILL_ON_FREE, &this->fill_on_free_bytes, nullptr, true),

    // Expand the size of every alloc by this number bytes. Value is
    // the total number of bytes to expand every allocation by.
    Feature ("expand_alloc", 16, 1, 16384, EXPAND_ALLOC, &this->expand_alloc_bytes,
             nullptr, false),

    // Keep track of the freed allocations and verify at a later date
    // that they have not been used. Turning this on, also turns on
    // fill on free.
    Feature("free_track", 100, 1, 16384, FREE_TRACK | FILL_ON_FREE, &this->free_track_allocations,
            nullptr, false),
    // Number of backtrace frames to keep when free_track is enabled. If this
    // value is set to zero, no backtrace will be kept.
    Feature("free_track_backtrace_num_frames", 16, 0, 256, 0,
            &this->free_track_backtrace_num_frames, nullptr, false),

    // Enable printing leaked allocations.
    Feature("leak_track", 0, 0, 0, LEAK_TRACK | TRACK_ALLOCS, nullptr, nullptr, false),
  };

  // Process each property name we can find.
  std::string property;
  size_t value;
  bool value_set;
  PropertyParser parser(property_str);
  bool found = false;
  bool valid = true;
  while (valid && parser.Get(&property, &value, &value_set)) {
    for (size_t i = 0; i < sizeof(features)/sizeof(Feature); i++) {
      if (property == features[i].name) {
        if (features[i].option == 0 && features[i].combo_option) {
          i++;
          for (; i < sizeof(features)/sizeof(Feature) && features[i].combo_option; i++) {
            if (!SetFeature(property, features[i], value, value_set)) {
              valid = false;
              break;
            }
            options |= features[i].option;
          }
          if (!valid) {
            break;
          }
        } else {
          if (!SetFeature(property, features[i], value, value_set)) {
            valid = false;
            break;
          }
          options |= features[i].option;
        }
        found = true;
        break;
      }
    }
    if (valid && !found) {
      error_log("%s: unknown option %s", getprogname(), property.c_str());
      valid = false;
      break;
    }
  }

  valid = valid && parser.Done();

  if (valid) {
    // It's necessary to align the front guard to MINIMUM_ALIGNMENT_BYTES to
    // make sure that the header is aligned properly.
    if (options & FRONT_GUARD) {
      front_guard_bytes = BIONIC_ALIGN(front_guard_bytes, MINIMUM_ALIGNMENT_BYTES);
    }

    // This situation can occur if the free_track option is specified and
    // the fill_on_free option is not. In this case, indicate the whole
    // allocation should be filled.
    if ((options & FILL_ON_FREE) && fill_on_free_bytes == 0) {
      fill_on_free_bytes = SIZE_MAX;
    }
  } else {
    parser.LogUsage();
  }

  return valid;
}
