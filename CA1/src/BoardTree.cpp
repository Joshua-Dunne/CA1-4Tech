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

void BoardTree::toRoot()
{
	m_current = m_root;
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

/// <summary>
/// Clean up any remaining Node pointers
/// </summary>
/// <param name="t_current">Current working Node pointer (Root at start)</param>
void BoardTree::cleanUp(Node* t_current)
{
	if (nullptr != t_current) // if the current working node isn't already null
	{
		// go through each child node recursively
		for (size_t i = 0; i < t_current->children.size(); i++)
		{
			if (nullptr != t_current->children[i]) // make sure this node isn't null already
				cleanUp(t_current->children[i]);
		}

	// once we reach here, that means we are at a node
	// that has no children, and that means
	// we can clean it up now

	
		// clear all vector data
		t_current->children.clear();

		// remove the reference to the parent
		t_current->parent = nullptr;

		// delete the pointer
		delete(t_current);
	}
	
}
