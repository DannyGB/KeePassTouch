#include "treenode.h"
#include "passwordentry.h"

TreeNode::TreeNode()
{
}

TreeNode::TreeNode(const PasswordEntry &passwordEntry, const PasswordEntry &next, const PasswordEntry &previous)
    : m_passwordEntry(passwordEntry), m_next(next), m_previous(previous)
{
}

PasswordEntry TreeNode::passwordEntry() const
{
    return m_passwordEntry;
}

PasswordEntry TreeNode::next() const
{
    return m_next;
}

PasswordEntry TreeNode::previous() const
{
    return m_previous;
}

bool TreeNode::isFirst()
{
    return true;
}

bool TreeNode::isLast()
{
    return true;
}
