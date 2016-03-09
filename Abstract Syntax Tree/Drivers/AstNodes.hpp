/******************************************************************\
 * Author: Trevor Sundberg
 * Copyright 2015, DigiPen Institute of Technology
\******************************************************************/

#pragma once
#ifndef COMPILER_CLASS_AST_NODES
#define COMPILER_CLASS_AST_NODES1

#include "Driver1.hpp"
#include <memory>
#include <vector>

// This is a helper template class that we use to simplify pushing unique pointers into a vector
// The push_back function returns a bool so that it can be used in conditions (such as a while loop)
// It also will only push when the unique_ptr is not null
template <typename T>
class unique_vector : public std::vector<std::unique_ptr<T>>
{
public:
  bool push_back(std::unique_ptr<T> ptr)
  {
    if (ptr)
    {
      vector::push_back(std::move(ptr));
      return true;
    }
    return false;
  }
};

// You must implement the base virtual Visitor class
class Visitor;

// Forward declarations of all the node types
class AbstractNode;
class BlockNode;
class ClassNode;
class StatementNode;
class VariableNode;
class ScopeNode;
class ParameterNode;
class FunctionNode;
class TypeNode;
class LabelNode;
class GotoNode;
class ReturnNode;
class BreakNode;
class ContinueNode;
class ExpressionNode;
class IfNode;
class WhileNode;
class ForNode;
class ValueNode;
class BinaryOperatorNode;
class UnaryOperatorNode;
class PostExpressionNode;
class MemberAccessNode;
class CallNode;
class CastNode;
class IndexNode;

// All of our node types inherit from this node and implement the Walk function
class AbstractNode
{
public:
  virtual ~AbstractNode();
  
  virtual void Walk(Visitor* visitor, bool visit = true);
};

class BlockNode : public AbstractNode
{
public:
  // ClassNode / VariableNode / FunctionNode
  unique_vector<AbstractNode> mGlobals;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class ClassNode : public AbstractNode
{
public:
  Token mName;
  
  // VariableNode / FunctionNode
  unique_vector<AbstractNode> mMembers;
  
  void Walk(Visitor* visitor, bool visit = true) override;
};

class StatementNode : public AbstractNode
{
public:

  void Walk(Visitor* visitor, bool visit = true) override;
};

class TypeNode : public AbstractNode
{
public:
  Token mName;
  size_t mPointerCount;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class VariableNode : public StatementNode
{
public:
  Token mName;
  std::unique_ptr<TypeNode> mType;

  // Can be null
  std::unique_ptr<ExpressionNode> mInitialValue;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class ScopeNode : public StatementNode
{
public:
  unique_vector<StatementNode> mStatements;
  
  void Walk(Visitor* visitor, bool visit = true) override;
};

class ParameterNode : public AbstractNode
{
public:
  Token mName;
  std::unique_ptr<TypeNode> mType;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class FunctionNode : public AbstractNode
{
public:
  Token mName;
  unique_vector<ParameterNode> mParameters;

  // Can be null
  std::unique_ptr<TypeNode> mReturnType;

  std::unique_ptr<ScopeNode> mScope;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class LabelNode : public StatementNode
{
public:
  Token mName;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class GotoNode : public StatementNode
{
public:
  Token mName;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class ReturnNode : public StatementNode
{
public:
  // Can be null
  std::unique_ptr<ExpressionNode> mReturnValue;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class BreakNode : public StatementNode
{
public:
  void Walk(Visitor* visitor, bool visit = true) override;
};

class ContinueNode : public StatementNode
{
public:
  void Walk(Visitor* visitor, bool visit = true) override;
};

class ExpressionNode : public StatementNode
{
public:
  void Walk(Visitor* visitor, bool visit = true) override;
};

class IfNode : public StatementNode
{
public:

  // Can be null
  std::unique_ptr<ExpressionNode> mCondition;

  std::unique_ptr<ScopeNode> mScope;

  // Can be null
  std::unique_ptr<IfNode> mElse;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class WhileNode : public StatementNode
{
public:
  std::unique_ptr<ExpressionNode> mCondition;
  std::unique_ptr<ScopeNode> mScope;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class ForNode : public StatementNode
{
public:
  // Can be null
  std::unique_ptr<VariableNode> mInitialVariable;
  // Can be null
  std::unique_ptr<ExpressionNode> mInitialExpression;

  // Can be null
  std::unique_ptr<ExpressionNode> mCondition;
  // Can be null
  std::unique_ptr<ExpressionNode> mIterator;

  std::unique_ptr<ScopeNode> mScope;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class ValueNode : public ExpressionNode
{
public:
  // Includes name references
  Token mToken;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class BinaryOperatorNode : public ExpressionNode
{
public:
  Token mOperator;
  std::unique_ptr<ExpressionNode> mLeft;
  std::unique_ptr<ExpressionNode> mRight;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class UnaryOperatorNode : public ExpressionNode
{
public:
  Token mOperator;
  std::unique_ptr<ExpressionNode> mRight;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class PostExpressionNode : public ExpressionNode
{
public:
  std::unique_ptr<ExpressionNode> mLeft;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class MemberAccessNode : public PostExpressionNode
{
public:
  Token mOperator;
  Token mName;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class CallNode : public PostExpressionNode
{
public:
  unique_vector<ExpressionNode> mArguments;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class CastNode : public PostExpressionNode
{
public:
  std::unique_ptr<TypeNode> mType;

  void Walk(Visitor* visitor, bool visit = true) override;
};

class IndexNode : public PostExpressionNode
{
public:
  std::unique_ptr<ExpressionNode> mIndex;

  void Walk(Visitor* visitor, bool visit = true) override;
};

#endif
