#ifndef TREENODE_H
#define TREENODE_H

#include "passwordentry.h"
#include <vector>

using namespace std;

class TreeNode
{
public:    
    TreeNode();
    TreeNode(const PasswordEntry &passwordEntry);
    TreeNode(const PasswordEntry &passwordEntry, TreeNode* parent);
    TreeNode(const PasswordEntry &passwordEntry, const vector<TreeNode*> &next, TreeNode* parent);
    PasswordEntry passwordEntry() const;
    void passwordEntry(const PasswordEntry &passwordEntry)
    {
        m_passwordEntry = passwordEntry;
    }

    vector<TreeNode*> next() const;
    void next(const vector<TreeNode*> &next)
    {
        m_next = next;
    }

    TreeNode* parent();
    void parent(TreeNode* parent)
    {
        m_parent = parent;
    }
    bool isFirst();
    bool isLast();

private:
    PasswordEntry m_passwordEntry;
    vector<TreeNode*> m_next;
    TreeNode* m_parent;
};

#endif // TREENODE_H
