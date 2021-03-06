#ifndef RENDER_CSSDATA_H
#define RENDER_CSSDATA_H

#include <map>
#include <string>
#include <vector>

#include "Wt/Render/CssData.h"

namespace Wt {
namespace Render {

struct SimpleSelectorImpl : public SimpleSelector
{
public:
  SimpleSelectorImpl(){}
  virtual std::string elementName() const { return elementName_; }
  virtual std::string hashId()      const { return hashid_; }
  virtual std::vector<std::string> classes() const { return classes_; }

  void setElementName(const std::string& name) { elementName_ = name; }
  void addClass      (const std::string& id)   { classes_.push_back(id); }
  void setHash       (const std::string& id)
                             { if(!hashid_.size()) hashid_ = id; }

  std::string elementName_;
  std::vector<std::string> classes_;
  std::string hashid_;
};

class SelectorImpl : public Selector
{
public:
  SelectorImpl(){}
  virtual unsigned int size()      const { return simpleSelectors_.size(); }
  virtual const SimpleSelector& at(int i) const{ return simpleSelectors_[i];}
  virtual Specificity specificity() const
  {
    // http://www.w3.org/TR/CSS21/cascade.html#specificity
    int a = 0, b = 0, c = 0, d = 0;
    for(unsigned int i = 0; i < simpleSelectors_.size(); ++i){
      // count 1 if the declaration is from is a 'style' attribute rather than a
      // rule with a selector, 0 otherwise (= a)
      // a = 0;
      // count the number of ID attributes in the selector (= b)
      if(simpleSelectors_[i].hashid_.size())
        ++b;
      // count the number of other attributes and pseudo-classes
      // in the selector (= c)
      c += simpleSelectors_[i].classes_.size();
      // count the number of element names and pseudo-elements
      // in the selector (= d)
      if(simpleSelectors_[i].elementName_.size()
         && simpleSelectors_[i].elementName_ != "*")
        ++d;
    }

    return Specificity(a, b, c, d);
  }

  void addSimpleSelector(const SimpleSelectorImpl& s)
         { simpleSelectors_.push_back(s); }

  std::vector<SimpleSelectorImpl> simpleSelectors_;
};

class DeclarationBlockImpl : public DeclarationBlock
{
public:
  DeclarationBlockImpl() { }
  virtual Term value(const std::string& property) const;
  virtual std::string declarationString() const { return declarationString_; }

  std::map<std::string, Term > properties_;
  std::string declarationString_;
};

class RulesetImpl : public Ruleset
{
public:
  RulesetImpl() { }
  virtual const Selector&         selector        () const { return selector_; }
  virtual const DeclarationBlock& declarationBlock() const { return block_; }

  void setBlock(const DeclarationBlockImpl& b){ block_ = b; }

  SelectorImpl          selector_;
  DeclarationBlockImpl  block_;
};

class StyleSheetImpl : public StyleSheet
{
public:
  StyleSheetImpl() { }
  virtual unsigned int   rulesetSize()    const { return rulesetArray_.size(); }
  virtual const Ruleset& rulesetAt(int i) const { return rulesetArray_[i]; }

  std::vector<RulesetImpl> rulesetArray_;
};

}
}



#endif
