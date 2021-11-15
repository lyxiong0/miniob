/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2021/5/14.
//

#ifndef __OBSERVER_SQL_EXECUTOR_VALUE_H_
#define __OBSERVER_SQL_EXECUTOR_VALUE_H_

#include <string.h>

#include <string>
#include <algorithm>
#include <ostream>
#include <iostream>
#include <functional>
#include <memory>

class TupleValue
{
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual int compare(const TupleValue &other) const = 0;
  virtual bool is_null() const = 0;
  virtual size_t to_hash() const = 0;
  virtual std::shared_ptr<TupleValue> clone() const = 0;

private:
};

class IntValue : public TupleValue
{
public:
  explicit IntValue(int value, bool is_null) : value_(value), is_null_(is_null)
  {
  }

  void to_string(std::ostream &os) const override
  {
    os << value_;
  }

  int compare(const TupleValue &other) const override
  {
    if (is_null_ || other.is_null())
    {
      return -1;
    }

    const IntValue &int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }

  int get_value()
  {
    return value_;
  }

  bool is_null() const override
  {
    return is_null_;
  }

  size_t to_hash() const override
  {
    return hash_func(value_);
  }

  std::shared_ptr<TupleValue> clone() const override
  {
    return std::make_shared<IntValue>(*this);
  }

private:
  int value_;
  bool is_null_;
  std::hash<int> hash_func;
};

class FloatValue : public TupleValue
{
public:
  explicit FloatValue(float value, bool is_null) : value_(value), is_null_(is_null)
  {
  }

  void to_string(std::ostream &os) const override
  {
    /*
    float输出规则：先保留两位小数（四舍五入），再去掉尾后0
    17.101 -> 17.10 -> 17.1
    */
    char ftos[50];
    sprintf(ftos, "%.2f", static_cast<float>(value_));
    int s_end = strlen(ftos) - 1;

    while (ftos[s_end] == '0')
    {
      --s_end;
    }

    if (ftos[s_end] == '.')
    {
      ftos[s_end] = '\0';
    }
    else
    {
      ftos[s_end + 1] = '\0';
    }

    os << ftos;
  }

  int compare(const TupleValue &other) const override
  {
    if (is_null_ || other.is_null())
    {
      return -1;
    }

    const FloatValue &float_other = (const FloatValue &)other;
    float result = value_ - float_other.value_;
    if (result > -1e-6 && result < 1e-6)
    {
      return 0;
    }
    else if (result > 0)
    { // 浮点数没有考虑精度问题
      return 1;
    }
    if (result < 0)
    {
      return -1;
    }
    return 0;
  }

  float get_value()
  {
    return value_;
  }

  bool is_null() const override
  {
    return is_null_;
  }

  size_t to_hash() const override
  {
    return hash_func(value_);
  }

  std::shared_ptr<TupleValue> clone() const override
  {
    return std::make_shared<FloatValue>(*this);
  }

private:
  float value_;
  bool is_null_;
  std::hash<float> hash_func;
};

class StringValue : public TupleValue
{
public:
  StringValue(const char *value, int len, bool is_null) : value_(value, len), is_null_(is_null)
  {
    remove_zero();
  }
  explicit StringValue(const char *value, bool is_null) : value_(value), is_null_(is_null)
  {
      
  }

  void remove_zero()
  {
    // 去掉最后的'\0'
    int i = value_.size() - 1;
    while (value_[i] == '\0') {
        i--;
    }
    value_ = value_.substr(0, i + 1);

    // 在这里处理中间的一端'\0'
    size_t first = value_.find_first_of('\0');
    size_t last = value_.find_last_of('\0');
    if (first != last) {
        size_t len = last - first + 1;
        // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!first_index " << first << std::endl;
        // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!last_index: " << last << std::endl;
        value_.erase(first, len);
    }
  }
  void to_string(std::ostream &os) const override
  {
    os << value_;
  }

  int compare(const TupleValue &other) const override
  {
    if (is_null_ || other.is_null())
    {
      return -1;
    }

    const StringValue &string_other = (const StringValue &)other;
    return strcmp(value_.c_str(), string_other.value_.c_str());
  }

  const char *get_value()
  {
    return value_.c_str();
  }

  int get_len()
  {
    return value_.size();
  }

  bool is_null() const override
  {
    return is_null_;
  }

  size_t to_hash() const override
  {
    return hash_func(value_);
  }

  std::shared_ptr<TupleValue> clone() const override
  {
    // 不能make_shared<基类>
    return std::make_shared<StringValue>(*this);
  }

private:
  std::string value_;
  bool is_null_;
  std::hash<std::string> hash_func;
};

#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
