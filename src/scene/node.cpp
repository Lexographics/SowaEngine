#include "node.hpp"

#include "core/application.hpp"

void Node::RemoveChild(Node *child) {
	child->_parent = nullptr;
	removeChild(child);
}

bool Node::Serialize(Document &doc) {
	doc.SetString("Type", App().GetNodeDB().GetNodeTypename(TypeID()));
	doc.SetString("Name", Name());
	doc.SetU64("ID", ID());

	return true;
}

bool Node::Deserialize(const Document &doc) {
	Rename(doc.GetString("Name", Name()));
	_id = doc.GetU64("ID", ID());

	return true;
}

bool Node::Copy(Node *dst) {
	dst->Rename(Name());
	return true;
}

void Node::AddChild(Node *child) {
	if (child->_parent != nullptr) {
		child->_parent->removeChild(child);
	}
	child->_parent = this;
	_children.push_back(child);
}

void Node::Free() {
	App().GetCurrentScene()->FreeNode(ID());
}

size_t Node::GetChildCount() {
	return _children.size();
}

Node *Node::GetChild(size_t index) {
	if (_children.size() >= index)
		return nullptr;
	return _children[index];
}

Node *Node::GetNode(const std::string nodePath, bool recursive) {
	std::filesystem::path path(nodePath);

	if (recursive) {
		auto nodePaths = Utils::Split(nodePath, "/");
		Node *node = this; // todo: if string starts with /, start it from root node
		for (auto path : nodePaths) {
			node = node->GetNode(path, false);
			if (nullptr == node)
				break;
		}
		return node;
	}

	for (Node *child : _children) {
		if (child->Name() == nodePath)
			return child;
	}
	return nullptr;
}

static void PrintIndent(int count) {
	for (int i = 0; i < count; i++) {
		std::cout << "=";
	}
	std::cout << "> ";
}

void Node::PrintHierarchy(int indent) {
	PrintIndent(indent);
	std::cout << Name() << " (" << App().GetNodeDB().GetNodeTypename(TypeID()) << ")" << std::endl;

	Node *node = this;
	for (Node *child : GetChildren()) {
		child->PrintHierarchy(indent + 1);
	}
}

void Node::removeChild(Node *child) {
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
}