#ifndef NODE_HPP
#define NODE_HPP
#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "sowa.hpp"
#include "utils/utils.hpp"

class Node {
  public:
	//
	inline NodeTypeID TypeID() const { return _typeid; }
	inline NodeID ID() const { return _id; }

	inline const std::string &Name() const { return _name; }
	inline void Rename(const std::string &name) { _name = name; }

	inline std::vector<Node *> GetChildren() { return _children; }
	inline Node *GetParent() { return _parent; }

	void RemoveChild(Node *child);
	void AddChild(Node *child);

	size_t GetChildCount();
	Node *GetChild(size_t index);

	Node *GetNode(const std::string nodePath, bool recursive = true);

	void PrintHierarchy(int indent = 0);

  private:
	// Internal hierarchy functions that does not modify other than the node passed
	void removeChild(Node *child);

	friend class Scene;
	friend class NodeDB;

	NodeTypeID _typeid = 0;
	NodeID _id = 0;

	std::string _name = "";

	Node *_parent = nullptr;
	std::vector<Node *> _children;
};

#endif // NODE_HPP