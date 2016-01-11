#ifndef __FORMATTER_HPP__

#define __FORMATTER_HPP__ 1

#include <iterator>
#include <string>
#include <vector>

namespace fmt {

using std::distance;
using std::string;
using std::vector;

template <typename T>
class Formatter : public T {
  public:
    using ann_t = typename T::result_type;

    using T::T;

    template<typename it_of_it_able_indices>
    ann_t format(const it_of_it_able_indices & cnf) {
      vector<ann_t> conjuncts;
      for(auto && c : cnf) conjuncts.push_back(this->fOr(c.begin(), c.end()));
      return this->And(conjuncts.begin(), conjuncts.end());
    }

};

template <typename ann_t>
class FormatterBase {
  public:
    using result_type = ann_t;

    virtual ~FormatterBase() {}
    FormatterBase(const vector<ann_t> & fs, const int o) : f(fs), k(o) {}

    virtual ann_t feature(const int & i) {
      return (i > 0 ? f[i-k] : this->Not(f[-i-k]));
    }

    virtual ann_t And(const ann_t &, const ann_t &) = 0;
    virtual ann_t Not(const ann_t &) = 0;
    virtual ann_t Or(const ann_t &, const ann_t &) = 0;

    virtual ann_t fAnd(const int &, const int &) = 0;
    virtual ann_t fNot(const int &) = 0;
    virtual ann_t fOr(const int &, const int &) = 0;

    template<class iter> ann_t And(iter, iter);
    template<class iter> ann_t Or(iter, iter);

    template<class iter> ann_t fAnd(iter, iter);
    template<class iter> ann_t fOr(iter, iter);

  protected:
    const int k;
    const vector<ann_t> & f;
};

namespace {

class Human : public FormatterBase<string> {
  public:
    using FormatterBase<string>::FormatterBase;

    string And(const string & a, const string & b) {
      return "(" + a + " AND " + b + ")";
    }

    string fAnd(const int & a, const unsigned & b) {
      return "(" + this->feature(a) + " AND " + this->feature(b) + ")";
    }

    template<class iter>
    string And(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      auto res = "(" + *a;
      for(++a; a != b; ++a) res += " AND " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string fAnd(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return this->feature(*a);
      auto res = "(" + this->feature(*a);
      for(++a; a != b; ++a) res += " AND " + this->feature(*a);
      res += ")";
      return res;
    }

    string Not(const string & a) {
      return "NOT(" + a + ")";
    }

    string fNot(const int & a) {
      return "NOT(" + this->feature(a) + ")";
    }

    string Or(const string & a, const string & b) {
      return "(" + a + " OR " + b + ")";
    }

    string fOr(const int & a, const int & b) {
      return "(" + this->feature(a) + " OR " + this->feature(b) + ")";
    }

    template<class iter>
    string Or(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      auto res = "(" + *a;
      for(++a; a != b; ++a) res += " OR " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string fOr(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return this->feature(*a);
      auto res = "(" + this->feature(*a);
      for(++a; a != b; ++a) res += " OR " + this->feature(*a);
      res += ")";
      return res;
    }

};

class SMTLIB2 : public FormatterBase<string> {
  public:
    using FormatterBase<string>::FormatterBase;

    string And(const string & a, const string & b) {
      return "(and " + a + " " + b + ")";
    }

    string fAnd(const int & a, const int & b) {
      return "(and " + this->feature(a) + " " + this->feature(b) + ")";
    }

    template<class iter>
    string And(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      string res = "(and";
      for(; a != b; ++a) res += " " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string fAnd(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return this->feature(*a);
      string res = "(and";
      for(; a != b; ++a) res += " " + this->feature(*a);
      res += ")";
      return res;
    }

    string Not(const string & a) {
      return "(not " + a + ")";
    }

    string fNot(const int & a) {
      return "(not " + this->feature(a) + ")";
    }

    string Or(const string & a, const string & b) {
      return "(or " + a + " " + b + ")";
    }

    string fOr(const int & a, const int & b) {
      return "(or " + this->feature(a) + " " + this->feature(b) + ")";
    }

    template<class iter>
    string Or(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return *a;
      string res = "(or";
      for(; a != b; ++a) res += " " + *a;
      res += ")";
      return res;
    }

    template<class iter>
    string fOr(iter a, iter b) {
      if(distance(a, b) == 0) return "";
      if(distance(a, b) < 2) return this->feature(*a);
      string res = "(or";
      for(; a != b; ++a) res += " " + this->feature(*a);
      res += ")";
      return res;
    }

};

}

using HumanFormatter = Formatter<Human>;
using SMTLIB2Formatter = Formatter<SMTLIB2>;

}

#endif
