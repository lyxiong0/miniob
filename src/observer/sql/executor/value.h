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

class TupleValue
{
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual int compare(const TupleValue &other) const = 0;
  virtual bool is_null() const = 0;

private:
};

class IntValue : public TupleValue
{
public:
  explicit IntValue(int value) : value_(value)
  {
  }

  void to_string(std::ostream &os) const override
  {
    os << value_;
  }

  int compare(const TupleValue &other) const override
  {
    const IntValue &int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }

  int GetValue()
  {
    return value_;
  }

  bool is_null() const override
  {
    return false;
  }

private:
  int value_;
};

class FloatValue : public TupleValue
{
public:
  explicit FloatValue(float value) : value_(value)
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

  float GetValue()
  {
    return value_;
  }

  bool is_null() const override
  {
    return false;
  }

private:
  float value_;
};

class StringValue : public TupleValue
{
public:
  StringValue(const char *value, int len) : value_(value, len)
  {
  }
  explicit StringValue(const char *value) : value_(value)
  {
  }

  void to_string(std::ostream &os) const override
  {
    os << value_;
  }

  int compare(const TupleValue &other) const override
  {
    const StringValue &string_other = (const StringValue &)other;
    return strcmp(value_.c_str(), string_other.value_.c_str());
  }

  const char *GetValue()
  {
    return value_.c_str();
  }

  int GetLen()
  {
    return value_.size();
  }

  bool is_null() const override
  {
    std::string tmp("Eu83");

    return value_ == tmp;
  }

private:
  std::string value_;
};

#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
