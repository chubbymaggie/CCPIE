#ifndef __FORMATTER_HPP__

#define __FORMATTER_HPP__ 1

#include <iterator>
#include <list>
#include <string>

//#include "operators.hpp"
#include "types.hpp"

namespace fmt {

using std::distance;
using std::string;
using std::list;

namespace {

template <typename ann_t>
class FormatterBase {
  public:
    using result_type = ann_t;

    virtual ann_t constant(const int &) = 0;
    virtual ann_t constant(const string &) = 0;
    virtual ann_t variable(const string &) = 0;

    virtual ann_t LT(const ann_t &, const ann_t &) = 0;
    virtual ann_t GT(const ann_t &, const ann_t &) = 0;
    virtual ann_t EQ(const ann_t &, const ann_t &) = 0;
    virtual ann_t NE(const ann_t &, const ann_t &) = 0;

    virtual ann_t AND(const ann_t &, const ann_t &) = 0;
    virtual ann_t NOT(const ann_t &) = 0;
    virtual ann_t OR(const ann_t &, const ann_t &) = 0;

    template<class iter> ann_t AND(iter, iter);
    template<class iter> ann_t OR(iter, iter);
};

class Human : public FormatterBase<string> {
  public:
    using FormatterBase<string>::FormatterBase;

    string constant(const int & a) { return std::to_string(a);  }
    string constant(const string & a) { return a;  }
    string variable(const string & a) { return a;  }

    string LT(const string & a, const string & b) { return OP(a, "<", b); }
    string GT(const string & a, const string & b) { return OP(a, ">", b); }
    string EQ(const string & a, const string & b) { return OP(a, "==", b); }
    string NE(const string & a, const string & b) { return OP(a, "!=", b); }

    string AND(const string & a, const string & b) { return OP(a, "&&", b); }
    string NOT(const string & a) { return "~(" + a + ")"; }
    string OR(const string & a, const string & b) { return OP(a, "||", b); }

    template<class iter>
    string AND(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      auto res = "(" + *a;
      for(++a; a != b; ++a) res += " AND " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string OR(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      auto res = "(" + *a;
      for(++a; a != b; ++a) res += " OR " + *a;
      res += ")";
      return res;
    }

  private:
    string OP(const string & a, const string & op, const string & b) {
      return "(" + a + " " + op + " " + b + ")";
    }
};

class SMTLIB2 : public FormatterBase<string> {
  public:
    using FormatterBase<string>::FormatterBase;

    /* TODO: declare variables too, to generate valid SMTLIB2 file */
    string constant(const int & a) { return std::to_string(a);  }
    string constant(const string & a) { return a;  }
    string variable(const string & a) { return a;  }

    string LT(const string & a, const string & b) { return OP("<", a, b); }
    string GT(const string & a, const string & b) { return OP(">", a, b); }
    string EQ(const string & a, const string & b) { return OP("=", a, b); }
    string NE(const string & a, const string & b) { return NOT(EQ(a, b)); }

    string AND(const string & a, const string & b) { return OP("and", a, b); }
    string NOT(const string & a) { return "(not " + a + ")"; }
    string OR(const string & a, const string & b) { return OP("or", a, b); }

    template<class iter>
    string AND(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      string res = "(and";
      for(; a != b; ++a) res += " " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string OR(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      string res = "(or";
      for(; a != b; ++a) res += " " + *a;
      res += ")";
      return res;
    }

  private:
    string OP(const string & op, const string & a, const string & b) {
      return "(" + op + " " + a + " " + b + ")";
    }
};

}

template <typename T>
class Formatter : public T {
  static_assert(types::is_base_of_template<T, FormatterBase>::value,
                "Unrecognized Formatter type!");

  public:
    using ann_t = typename T::result_type;

    using T::T;

    template<typename it_of_it_able_indices>
    ann_t format(const it_of_it_able_indices & cnf) {
      list<ann_t> conjuncts;
      for(auto && c : cnf) conjuncts.push_back(this->OR(c.begin(), c.end()));
      return this->AND(conjuncts.begin(), conjuncts.end());
    }

};

using HumanFormatter = Formatter<Human>;
using SMTLIB2Formatter = Formatter<SMTLIB2>;

}

#endif
