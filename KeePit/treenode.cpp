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
