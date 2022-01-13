#include "..\include\BoardTree.h"

void BoardTree::setRoot(Node* t_root)
{
	m_root = t_root;
	m_current = t_root;
}

void BoardTree::setCurrent(Node* t_curr)
{
	if (nullptr != t_curr)
	{
		m_current = t_curr;
	}
}

void BoardTree::addChild(Node* t_child)
{
	m_current->children.push_back(t_child);
	t_child->parent = m_current;
}

Node* BoardTree::toRoot()
{
	m_current = m_root;
	return m_current;
}

Node* BoardTree::moveTo(int t_child)
{
	m_current = m_current->children[t_child];

	return m_current;
}

Node* BoardTree::moveUp()
{
	if (nullptr != m_current)
	{
		return nullptr != m_current->parent ? m_current->parent : nullptr;
	}

	return nullptr;
}
