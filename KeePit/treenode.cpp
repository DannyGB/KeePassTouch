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

///
/// \brief TreeNode::TreeNode
///
TreeNode::TreeNode()
    : m_parent(0)
{
}

///
/// \brief TreeNode::TreeNode
/// \param passwordEntry
///
TreeNode::TreeNode(const PasswordEntry &passwordEntry)
    : m_passwordEntry(passwordEntry), m_parent(0)
{    
}

///
/// \brief TreeNode::TreeNode
/// \param passwordEntry
/// \param parent
///
TreeNode::TreeNode(const PasswordEntry &passwordEntry, TreeNode *parent)
    : m_passwordEntry(passwordEntry), m_parent(parent)
{
}

///
/// \brief TreeNode::TreeNode
/// \param passwordEntry
/// \param next
/// \param parent
///
TreeNode::TreeNode(const PasswordEntry &passwordEntry, const vector<TreeNode*> &next, TreeNode *parent)
    : m_passwordEntry(passwordEntry), m_next(next), m_parent(parent)
{
}

///
/// \brief TreeNode::passwordEntry
/// \return
///
PasswordEntry TreeNode::passwordEntry() const
{
    return m_passwordEntry;
}

///
/// \brief TreeNode::next
/// \return
///
vector<TreeNode*> TreeNode::next() const
{
    return m_next;
}

///
/// \brief TreeNode::parent
/// \return
///
TreeNode* TreeNode::parent()
{
    return m_parent;
}

///
/// \brief TreeNode::isFirst
/// \return
///
bool TreeNode::isFirst()
{
    return true;
}

///
/// \brief TreeNode::isLast
/// \return
///
bool TreeNode::isLast()
{
    return true;
}
