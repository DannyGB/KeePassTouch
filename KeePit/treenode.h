/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
