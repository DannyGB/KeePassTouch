#include "treenode.h"
#include "passwordentry.h"

using namespace std;

TreeNode::TreeNode()
    : m_parent(0)
{
}

TreeNode::TreeNode(const PasswordEntry &passwordEntry)
    : m_passwordEntry(passwordEntry), m_parent(0)
{    
}

TreeNode::TreeNode(const PasswordEntry &passwordEntry, TreeNode *parent)
    : m_passwordEntry(passwordEntry), m_parent(parent)
{
}

TreeNode::TreeNode(const PasswordEntry &passwordEntry, const vector<TreeNode*> &next, TreeNode *parent)
    : m_passwordEntry(passwordEntry), m_next(next), m_parent(parent)
{
}

PasswordEntry TreeNode::passwordEntry() const
{
    return m_passwordEntry;
}

vector<TreeNode*> TreeNode::next() const
{
    return m_next;
}

TreeNode* TreeNode::parent()
{
    return m_parent;
}

bool TreeNode::isFirst()
{
    return true;
}

bool TreeNode::isLast()
{
    return true;
}
