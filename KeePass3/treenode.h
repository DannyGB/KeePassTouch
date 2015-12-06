#ifndef TREENODE_H
#define TREENODE_H

#include "passwordentry.h"

class TreeNode
{
public:
    TreeNode();
    TreeNode(const PasswordEntry &passwordEntry, const PasswordEntry &next, const PasswordEntry &previous);
    PasswordEntry passwordEntry() const;
    void passwordEntry(const PasswordEntry &passwordEntry)
    {
        m_passwordEntry = passwordEntry;
    }

    PasswordEntry next() const;
    void next(const PasswordEntry &next)
    {
        m_passwordEntry = next;
    }
    PasswordEntry previous() const;
    void previous(const PasswordEntry &previous)
    {
        m_passwordEntry = previous;
    }
    bool isFirst();
    bool isLast();
private:
    PasswordEntry m_passwordEntry;
    PasswordEntry m_next;
    PasswordEntry m_previous;
};

#endif // TREENODE_H
