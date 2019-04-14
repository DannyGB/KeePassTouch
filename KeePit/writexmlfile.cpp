/*
* This file is part of KeePit
*
* Copyright (C) 2016-2018 Dan Beavon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "writexmlfile.h"
#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "base64.h"

#include <assert.h>
#include <vector>
#include <QUuid>

#include "hexx.h"
#include "./tinyxml2.h"

using namespace tinyxml2;

///
/// \brief WriteXmlFile::WriteXmlFile
/// \param xml
/// \param size
/// \param salsa
///
WriteXmlFile::WriteXmlFile(const char* xml, size_t size, Salsa20 *salsa)
{
    m_xml = xml;
    m_size = size;
    m_salsa = salsa;
}

///
/// \brief WriteXmlFile::~WriteXmlFile
///
WriteXmlFile::~WriteXmlFile()
{
    delete m_salsa;
    m_salsa = 0;
    delete lastRead;
    lastRead = 0;
}

///
/// \brief WriteXmlFile::ToString
/// \return
///
void WriteXmlFile::CreateNewDatabase(QString filePath)
{
    Base64 base64;

    XMLDocument doc;
    XMLDeclaration * decl = doc.NewDeclaration();
    doc.InsertFirstChild(decl);

	XMLElement * keepassFile = doc.NewElement( "KeePassFile" ); // everything hangs off this element
    doc.InsertEndChild(keepassFile);

    XMLElement * meta = doc.NewElement( "Meta" ); // meta and root are sibling elements
    keepassFile->InsertEndChild( meta );

    XMLElement * generator = doc.NewElement( "Generator" );
    generator->SetText("KeePass");
    meta->InsertEndChild( generator );
    XMLElement * databaseName = doc.NewElement( "DatabaseName" );
    databaseName->SetText("KeePass database");
    meta->InsertEndChild( databaseName );
    XMLElement * DatabaseNameChanged = doc.NewElement( "DatabaseNameChanged" );

    char nowBuff[70];
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm now_tm = *std::localtime(&now_c);
    strftime(nowBuff, sizeof nowBuff, "%Y-%m-%dT%H:%M:%SZ", &now_tm);
    DatabaseNameChanged->SetText(nowBuff);
    meta->InsertEndChild( DatabaseNameChanged );

    XMLElement * DatabaseDescription = doc.NewElement( "DatabaseDescription" );
    meta->InsertEndChild( DatabaseDescription );
    XMLElement * DatabaseDescriptionChanged = doc.NewElement( "DatabaseDescriptionChanged" );
    DatabaseDescriptionChanged->SetText(nowBuff);
    meta->InsertEndChild( DatabaseDescriptionChanged );

    XMLElement * DefaultUserName = doc.NewElement( "DefaultUserName" );
    meta->InsertEndChild( DefaultUserName );

    XMLElement * DefaultUserNameChanged = doc.NewElement( "DefaultUserNameChanged" );
    DefaultUserNameChanged->SetText(nowBuff);
    meta->InsertEndChild( DefaultUserNameChanged );

    XMLElement * MaintenanceHistoryDays = doc.NewElement( "MaintenanceHistoryDays" );
    MaintenanceHistoryDays->SetText(365);
    meta->InsertEndChild( MaintenanceHistoryDays );

    XMLElement * Color = doc.NewElement( "Color" );
    meta->InsertEndChild( Color );

    XMLElement * MasterKeyChanged = doc.NewElement( "MasterKeyChanged" );
    MasterKeyChanged->SetText(nowBuff);
    meta->InsertEndChild( MasterKeyChanged );

    XMLElement * MasterKeyChangeRec = doc.NewElement( "MasterKeyChangeRec" );
    MasterKeyChangeRec ->SetText("-1");
    meta->InsertEndChild( MasterKeyChangeRec );

    XMLElement * MasterKeyChangeForce = doc.NewElement( "MasterKeyChangeForce" );
    MasterKeyChangeForce ->SetText(1);
    meta->InsertEndChild( MasterKeyChangeForce );

    XMLElement * MemoryProtection = doc.NewElement( "MemoryProtection" );
    meta->InsertEndChild( MemoryProtection );
    XMLElement * ProtectTitle = doc.NewElement( "ProtectTitle" );
    ProtectTitle->SetText("False");
    MemoryProtection->InsertEndChild( ProtectTitle );
    XMLElement * ProtectUserName = doc.NewElement( "ProtectUserName" );
    ProtectUserName->SetText("False");
    MemoryProtection->InsertEndChild( ProtectUserName );
    XMLElement * ProtectPassword = doc.NewElement( "ProtectPassword" );
    ProtectPassword->SetText("True");
    MemoryProtection->InsertEndChild( ProtectPassword );
    XMLElement * ProtectURL = doc.NewElement( "ProtectURL" );
    ProtectURL->SetText("False");
    MemoryProtection->InsertEndChild( ProtectURL );
    XMLElement * ProtectNotes = doc.NewElement( "ProtectNotes" );
    ProtectNotes->SetText("False");
    MemoryProtection->InsertEndChild( ProtectNotes );

    XMLElement * RecycleBinEnabled = doc.NewElement( "RecycleBinEnabled" );
    RecycleBinEnabled->SetText("True");
    meta->InsertEndChild( RecycleBinEnabled );
    XMLElement * RecycleBinUUID = doc.NewElement( "RecycleBinUUID" );
    RecycleBinUUID->SetText("AAAAAAAAAAAAAAAAAAAAAA==");
    meta->InsertEndChild( RecycleBinUUID );
    XMLElement * RecycleBinChanged = doc.NewElement( "RecycleBinChanged" );
    RecycleBinChanged->SetText(nowBuff);
    meta->InsertEndChild( RecycleBinChanged );
    XMLElement * EntryTemplatesGroup = doc.NewElement( "EntryTemplatesGroup" );
    EntryTemplatesGroup->SetText("AAAAAAAAAAAAAAAAAAAAAA==");
    meta->InsertEndChild( EntryTemplatesGroup );
    XMLElement * EntryTemplatesGroupChanged = doc.NewElement( "EntryTemplatesGroupChanged" );
    EntryTemplatesGroupChanged->SetText(nowBuff);
    meta->InsertEndChild( EntryTemplatesGroupChanged );
    XMLElement * HistoryMaxItems = doc.NewElement( "HistoryMaxItems" );
    HistoryMaxItems->SetText(10);
    meta->InsertEndChild( HistoryMaxItems );
    XMLElement * HistoryMaxSize = doc.NewElement( "HistoryMaxSize" );
    HistoryMaxSize->SetText(621456);
    meta->InsertEndChild( HistoryMaxSize );
    XMLElement * LastSelectedGroup = doc.NewElement( "LastSelectedGroup" );
    LastSelectedGroup->SetText("AAAAAAAAAAAAAAAAAAAAAA==");
    meta->InsertEndChild( LastSelectedGroup );
    XMLElement * LastTopVisibleGroup = doc.NewElement( "LastTopVisibleGroup" );
    LastTopVisibleGroup->SetText("AAAAAAAAAAAAAAAAAAAAAA==");
    meta->InsertEndChild( LastTopVisibleGroup );
    XMLElement * Binaries = doc.NewElement( "Binaries" );
    meta->InsertEndChild( Binaries );
    XMLElement * CustomData = doc.NewElement( "CustomData" );
    meta->InsertEndChild( CustomData );

    XMLElement * root = doc.NewElement( "Root" );
    keepassFile->InsertEndChild( root );

    XMLElement * group = doc.NewElement( "Group" ); // Group element hangs off root
    root->InsertEndChild( group );

    XMLElement * deletedObjects = doc.NewElement( "DeletedObjects" );
    root->InsertEndChild( deletedObjects );

    XMLElement * uuid = doc.NewElement( "UUID" ); // hangs off Group
    
    QUuid uid = QUuid::createUuid();
    QString uuidStr = uid.toString();
    
    uuidStr.remove("{");
    uuidStr.remove("}");
    uuidStr.remove("-");

    QByteArray strBytes = Hex::HexStringToByteArray(uuidStr);
    
    std::string uStr = base64.base64_encode(strBytes.data(), strBytes.size());
    uuid->SetText(uStr.c_str());
    group->InsertEndChild( uuid );

    // debugging info
    // XMLElement * uuid_temp = doc.NewElement( "uuid_temp" ); // hangs off Group
    // uuid_temp->SetText(uuidStr.toStdString().c_str());
    // group->InsertEndChild( uuid_temp );


    XMLElement * name = doc.NewElement( "Name" ); // hangs off Group
    name->SetText("keepass");
    group->InsertEndChild( name );
    XMLElement * notes = doc.NewElement( "Notes" ); // hangs off Group
    group->InsertEndChild( notes );

    XMLElement * times = doc.NewElement( "Times" ); // hangs off Group
    group->InsertEndChild( times );
    XMLElement * creationTime = doc.NewElement( "CreationTime" ); // hangs off Times
    creationTime->SetText(nowBuff);
    times->InsertEndChild( creationTime );
    XMLElement * lastModificationTime = doc.NewElement( "LastModificationTime" ); // hangs off Times
    lastModificationTime->SetText(nowBuff);
    times->InsertEndChild( lastModificationTime );
    XMLElement * lastAccessTime = doc.NewElement( "LastAccessTime" ); // hangs off Times
    lastAccessTime->SetText(nowBuff);
    times->InsertEndChild( lastAccessTime );
    XMLElement * expiryTime = doc.NewElement( "ExpiryTime" ); // hangs off Times
    expiryTime->SetText(nowBuff);
    times->InsertEndChild( expiryTime );
    XMLElement * expires = doc.NewElement( "Expires" ); // hangs off Times
    expires->SetText("False");
    times->InsertEndChild( expires );
    XMLElement * usageCount = doc.NewElement( "UsageCount" ); // hangs off Times
    usageCount->SetText(0);
    times->InsertEndChild( usageCount );
    XMLElement * locationChanged = doc.NewElement( "LocationChanged" ); // hangs off Times
    locationChanged->SetText(nowBuff);
    times->InsertEndChild( locationChanged );

    XMLElement * isExpanded = doc.NewElement( "IsExpanded" ); // hangs off Group
    isExpanded->SetText("True");
    group->InsertEndChild( isExpanded );
    XMLElement * defaultAutoTypeSequence = doc.NewElement( "DefaultAutoTypeSequence" ); // hangs off Group
    group->InsertEndChild( defaultAutoTypeSequence );
    XMLElement * enableAutoType = doc.NewElement( "EnableAutoType" ); // hangs off Group
    enableAutoType->SetText("null");
    group->InsertEndChild( enableAutoType );
    XMLElement * enableSearching = doc.NewElement( "EnableSearching" ); // hangs off Group
    enableSearching->SetText("null");
    group->InsertEndChild( enableSearching );
    XMLElement * lastTopVisibleEntry = doc.NewElement( "LastTopVisibleEntry" ); // hangs off Group
    lastTopVisibleEntry->SetText("AAAAAAAAAAAAAAAAAAAAAA==");
    group->InsertEndChild( lastTopVisibleEntry );

    //XmlText * text = new TiXmlText( "World" );
	//doc.SaveFile( filePath.toLocal8Bit().data() );
}

///
/// \brief WriteXmlFile::ToString
/// \return
///
/*QString WriteXmlFile::ToString(vector<TreeNode*>)
{
}*/
