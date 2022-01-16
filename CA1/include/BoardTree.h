#ifndef BOARD_TREE_H
#define BOARD_TREE_H

#include <iostream>
#include <vector>
#include "Board.h"

struct Node
{
	Node(int t_x, int t_y, int t_val, Board t_brd) : x(t_x), y(t_y), value(t_val), board(t_brd) {};
	int x = 0;
	int y = 0;
	int value = 0;
	Board board;
	Node* parent = nullptr;
	std::vector<Node*> children;
};

class BoardTree
{
public:
	void setRoot(Node* t_root);
	void setCurrent(Node* t_curr);
	void addChild(Node* t_child);

	void toRoot();
	inline Node* getRoot() { return m_root; };
	inline int getMaxChildren() { return m_current->children.size(); };
	Node* moveTo(int t_child);
	Node* moveUp();
	void cleanUp(Node* t_current);

	Node* m_current = nullptr;

private:
	Node* m_root = nullptr;
};

#endif