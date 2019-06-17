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
#include "uuid.h"
#include "./tinyxml2.h"

using namespace tinyxml2;

const char * WriteXmlFile::LASTTOPVISIBLEENTRY = "LastTopVisibleEntry";
const char * WriteXmlFile::ENABLESEARCHING = "EnableSearching";
const char * WriteXmlFile::ENABLEAUTOTYPE = "EnableAutoType";
const char * WriteXmlFile::DEFAULTAUTOTYPESEQUENCE = "DefaultAutoTypeSequence";
const char * WriteXmlFile::ISEXPANDED = "IsExpanded";
const char * WriteXmlFile::LOCATIONCHANGED = "LocationChanged";
const char * WriteXmlFile::USAGECOUNT = "UsageCount";
const char * WriteXmlFile::EXPIRES = "Expires";
const char * WriteXmlFile::EXPIRYTIME = "ExpiryTime";
const char * WriteXmlFile::LASTACCESSTIME = "LastAccessTime";
const char * WriteXmlFile::LASTMODIFICATIONTIME = "LastModificationTime";
const char * WriteXmlFile::CREATIONTIME = "CreationTime";
const char * WriteXmlFile::TIMES = "Times";
const char * WriteXmlFile::NOTES = "Notes";
const char * WriteXmlFile::NAME = "Name";
const char * WriteXmlFile::UUID = "UUID";
const char * WriteXmlFile::DELETEDOBJECTS = "DeletedObjects";
const char * WriteXmlFile::GROUP = "Group";
const char * WriteXmlFile::ROOT = "Root";
const char * WriteXmlFile::CUSTOMDATA = "CustomData";
const char * WriteXmlFile::BINARIES = "Binaries";
const char * WriteXmlFile::LASTSELECTEDGROUP = "LastSelectedGroup";
const char * WriteXmlFile::LASTTOPVISIBLEGROUP = "LastTopVisibleGroup";
const char * WriteXmlFile::HISTORYMAXSIZE = "HistoryMaxSize";
const char * WriteXmlFile::HISTORYMAXITEMS = "HistoryMaxItems";
const char * WriteXmlFile::ENTRYTEMPLATESGROUPCHANGED = "EntryTemplatesGroupChanged";
const char * WriteXmlFile::ENTRYTEMPLATESGROUP = "EntryTemplatesGroup";
const char * WriteXmlFile::RECYCLEBINCHANGED = "RecycleBinChanged";
const char * WriteXmlFile::RECYCLEBINUUID = "RecycleBinUUID";
const char * WriteXmlFile::RECYCLEBINENABLED = "RecycleBinEnabled";
const char * WriteXmlFile::PROTECTNOTES = "ProtectNotes";
const char * WriteXmlFile::PROTECTURL = "ProtectURL";
const char * WriteXmlFile::PROTECTPASSWORD = "ProtectPassword";
const char * WriteXmlFile::PROTECTUSERNAME = "ProtectUserName";
const char * WriteXmlFile::PROTECTTITLE = "ProtectTitle";
const char * WriteXmlFile::MEMORYPROTECTION = "MemoryProtection";
const char * WriteXmlFile::MASTERKEYCHANGEFORCE = "MasterKeyChangeForce";
const char * WriteXmlFile::MASTERKEYCHANGEREC = "MasterKeyChangeRec";
const char * WriteXmlFile::MASTERKEYCHANGED = "MasterKeyChanged";
const char * WriteXmlFile::COLOR = "Color";
const char * WriteXmlFile::MAINTENANCEHISTORYDAYS = "MaintenanceHistoryDays";
const char * WriteXmlFile::DEFAULTUSERNAMECHANGED = "DefaultUserNameChanged";
const char * WriteXmlFile::DEFAULTUSERNAME = "DefaultUserName";
const char * WriteXmlFile::DATABASEDESCRIPTIONCHANGED = "DatabaseDescriptionChanged";
const char * WriteXmlFile::DATABASEDESCRIPTION = "DatabaseDescription";
const char * WriteXmlFile::DATABASENAMECHANGED = "DatabaseNameChanged";
const char * WriteXmlFile::DATABASENAME = "DatabaseName";
const char * WriteXmlFile::GENERATOR = "Generator";
const char * WriteXmlFile::META = "Meta";
const char * WriteXmlFile::KEEPASSFILE = "KeePassFile";

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
QString WriteXmlFile::CreateNewDatabase(QString filePath)
{
    Base64 base64;

    XMLDocument doc;
    XMLDeclaration * decl = doc.NewDeclaration();
    doc.InsertFirstChild(decl);

	XMLElement * keepassFile = doc.NewElement( WriteXmlFile::KEEPASSFILE ); // everything hangs off this element
    doc.InsertEndChild(keepassFile);

    XMLElement * meta = doc.NewElement( WriteXmlFile::META ); // meta and root are sibling elements
    keepassFile->InsertEndChild( meta );

    XMLElement * generator = doc.NewElement( WriteXmlFile::GENERATOR );
    generator->SetText("KeePass");
    meta->InsertEndChild( generator );
    XMLElement * databaseName = doc.NewElement( WriteXmlFile::DATABASENAME );
    databaseName->SetText("KeePass database");
    meta->InsertEndChild( databaseName );
    XMLElement * DatabaseNameChanged = doc.NewElement( WriteXmlFile::DATABASENAMECHANGED );

    char nowBuff[70];
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm now_tm = *std::localtime(&now_c);
    strftime(nowBuff, sizeof nowBuff, "%Y-%m-%dT%H:%M:%SZ", &now_tm);
    DatabaseNameChanged->SetText(nowBuff);
    meta->InsertEndChild( DatabaseNameChanged );

    XMLElement * DatabaseDescription = doc.NewElement( WriteXmlFile::DATABASEDESCRIPTION );
    meta->InsertEndChild( DatabaseDescription );
    XMLElement * DatabaseDescriptionChanged = doc.NewElement( WriteXmlFile::DATABASEDESCRIPTIONCHANGED );
    DatabaseDescriptionChanged->SetText(nowBuff);
    meta->InsertEndChild( DatabaseDescriptionChanged );

    XMLElement * DefaultUserName = doc.NewElement( WriteXmlFile::DEFAULTUSERNAME );
    meta->InsertEndChild( DefaultUserName );

    XMLElement * DefaultUserNameChanged = doc.NewElement( WriteXmlFile::DEFAULTUSERNAMECHANGED );
    DefaultUserNameChanged->SetText(nowBuff);
    meta->InsertEndChild( DefaultUserNameChanged );

    XMLElement * MaintenanceHistoryDays = doc.NewElement( WriteXmlFile::MAINTENANCEHISTORYDAYS );
    MaintenanceHistoryDays->SetText(365);
    meta->InsertEndChild( MaintenanceHistoryDays );

    XMLElement * Color = doc.NewElement( WriteXmlFile::COLOR );
    meta->InsertEndChild( Color );

    XMLElement * MasterKeyChanged = doc.NewElement( WriteXmlFile::MASTERKEYCHANGED );
    MasterKeyChanged->SetText(nowBuff);
    meta->InsertEndChild( MasterKeyChanged );

    XMLElement * MasterKeyChangeRec = doc.NewElement( WriteXmlFile::MASTERKEYCHANGEREC );
    MasterKeyChangeRec ->SetText("-1");
    meta->InsertEndChild( MasterKeyChangeRec );

    XMLElement * MasterKeyChangeForce = doc.NewElement( WriteXmlFile::MASTERKEYCHANGEFORCE );
    MasterKeyChangeForce ->SetText(1);
    meta->InsertEndChild( MasterKeyChangeForce );

    XMLElement * MemoryProtection = doc.NewElement( WriteXmlFile::MEMORYPROTECTION );
    meta->InsertEndChild( MemoryProtection );
    XMLElement * ProtectTitle = doc.NewElement( WriteXmlFile::PROTECTTITLE );
    ProtectTitle->SetText("False");
    MemoryProtection->InsertEndChild( ProtectTitle );
    XMLElement * ProtectUserName = doc.NewElement( WriteXmlFile::PROTECTUSERNAME );
    ProtectUserName->SetText("False");
    MemoryProtection->InsertEndChild( ProtectUserName );
    XMLElement * ProtectPassword = doc.NewElement( WriteXmlFile::PROTECTPASSWORD );
    ProtectPassword->SetText("True");
    MemoryProtection->InsertEndChild( ProtectPassword );
    XMLElement * ProtectURL = doc.NewElement( WriteXmlFile::PROTECTURL );
    ProtectURL->SetText("False");
    MemoryProtection->InsertEndChild( ProtectURL );
    XMLElement * ProtectNotes = doc.NewElement( WriteXmlFile::PROTECTNOTES );
    ProtectNotes->SetText("False");
    MemoryProtection->InsertEndChild( ProtectNotes );

    XMLElement * RecycleBinEnabled = doc.NewElement( WriteXmlFile::RECYCLEBINENABLED );
    RecycleBinEnabled->SetText("True");
    meta->InsertEndChild( RecycleBinEnabled );
    XMLElement * RecycleBinUUID = doc.NewElement( WriteXmlFile::RECYCLEBINUUID  );
    RecycleBinUUID->SetText(UUID::EMPTYUUIDSTRING);
    meta->InsertEndChild( RecycleBinUUID );
    XMLElement * RecycleBinChanged = doc.NewElement( WriteXmlFile::RECYCLEBINCHANGED );
    RecycleBinChanged->SetText(nowBuff);
    meta->InsertEndChild( RecycleBinChanged );
    XMLElement * EntryTemplatesGroup = doc.NewElement( WriteXmlFile::ENTRYTEMPLATESGROUP  );
    EntryTemplatesGroup->SetText(UUID::EMPTYUUIDSTRING);
    meta->InsertEndChild( EntryTemplatesGroup );
    XMLElement * EntryTemplatesGroupChanged = doc.NewElement( WriteXmlFile::ENTRYTEMPLATESGROUPCHANGED );
    EntryTemplatesGroupChanged->SetText(nowBuff);
    meta->InsertEndChild( EntryTemplatesGroupChanged );
    XMLElement * HistoryMaxItems = doc.NewElement( WriteXmlFile::HISTORYMAXITEMS );
    HistoryMaxItems->SetText(10);
    meta->InsertEndChild( HistoryMaxItems );
    XMLElement * HistoryMaxSize = doc.NewElement( WriteXmlFile::HISTORYMAXSIZE );
    HistoryMaxSize->SetText(621456);
    meta->InsertEndChild( HistoryMaxSize );
    XMLElement * LastSelectedGroup = doc.NewElement( WriteXmlFile::LASTSELECTEDGROUP );
    LastSelectedGroup->SetText(UUID::EMPTYUUIDSTRING);
    meta->InsertEndChild( LastSelectedGroup );
    XMLElement * LastTopVisibleGroup = doc.NewElement( WriteXmlFile::LASTTOPVISIBLEGROUP );
    LastTopVisibleGroup->SetText(UUID::EMPTYUUIDSTRING);
    meta->InsertEndChild( LastTopVisibleGroup );
    XMLElement * Binaries = doc.NewElement( WriteXmlFile::BINARIES );
    meta->InsertEndChild( Binaries );
    XMLElement * CustomData = doc.NewElement( WriteXmlFile::CUSTOMDATA );
    meta->InsertEndChild( CustomData );

    XMLElement * root = doc.NewElement( WriteXmlFile::ROOT );
    keepassFile->InsertEndChild( root );

    XMLElement * group = doc.NewElement( WriteXmlFile::GROUP ); // Group element hangs off root
    root->InsertEndChild( group );

    XMLElement * deletedObjects = doc.NewElement( WriteXmlFile::DELETEDOBJECTS );
    root->InsertEndChild( deletedObjects );

    XMLElement * uuid = doc.NewElement( WriteXmlFile::UUID ); // hangs off Group
    QByteArray strBytes = UUID::CreateUUID();
    std::string uStr = base64.base64_encode(strBytes.data(), strBytes.size());
    uuid->SetText(uStr.c_str());
    group->InsertEndChild( uuid );

    // debugging info
    // XMLElement * uuid_temp = doc.NewElement( "uuid_temp" ); // hangs off Group
    // uuid_temp->SetText(uuidStr.toStdString().c_str());
    // group->InsertEndChild( uuid_temp );


    XMLElement * name = doc.NewElement( WriteXmlFile::NAME ); // hangs off Group
    name->SetText("keepass");
    group->InsertEndChild( name );
    XMLElement * notes = doc.NewElement( WriteXmlFile::NOTES ); // hangs off Group
    group->InsertEndChild( notes );

    XMLElement * times = doc.NewElement( WriteXmlFile::TIMES ); // hangs off Group
    group->InsertEndChild( times );
    XMLElement * creationTime = doc.NewElement( WriteXmlFile::CREATIONTIME ); // hangs off Times
    creationTime->SetText(nowBuff);
    times->InsertEndChild( creationTime );
    XMLElement * lastModificationTime = doc.NewElement( WriteXmlFile::LASTMODIFICATIONTIME ); // hangs off Times
    lastModificationTime->SetText(nowBuff);
    times->InsertEndChild( lastModificationTime );
    XMLElement * lastAccessTime = doc.NewElement( WriteXmlFile::LASTACCESSTIME  ); // hangs off Times
    lastAccessTime->SetText(nowBuff);
    times->InsertEndChild( lastAccessTime );
    XMLElement * expiryTime = doc.NewElement( WriteXmlFile::EXPIRYTIME ); // hangs off Times
    expiryTime->SetText(nowBuff);
    times->InsertEndChild( expiryTime );
    XMLElement * expires = doc.NewElement( WriteXmlFile::EXPIRES ); // hangs off Times
    expires->SetText("False");
    times->InsertEndChild( expires );
    XMLElement * usageCount = doc.NewElement( WriteXmlFile::USAGECOUNT ); // hangs off Times
    usageCount->SetText(0);
    times->InsertEndChild( usageCount );
    XMLElement * locationChanged = doc.NewElement( WriteXmlFile::LOCATIONCHANGED ); // hangs off Times
    locationChanged->SetText(nowBuff);
    times->InsertEndChild( locationChanged );

    XMLElement * isExpanded = doc.NewElement( WriteXmlFile::ISEXPANDED ); // hangs off Group
    isExpanded->SetText("True");
    group->InsertEndChild( isExpanded );
    XMLElement * defaultAutoTypeSequence = doc.NewElement( WriteXmlFile::DEFAULTAUTOTYPESEQUENCE ); // hangs off Group
    group->InsertEndChild( defaultAutoTypeSequence );
    XMLElement * enableAutoType = doc.NewElement( WriteXmlFile::ENABLEAUTOTYPE ); // hangs off Group
    enableAutoType->SetText("null");
    group->InsertEndChild( enableAutoType );
    XMLElement * enableSearching = doc.NewElement( WriteXmlFile::ENABLESEARCHING ); // hangs off Group
    enableSearching->SetText("null");
    group->InsertEndChild( enableSearching );
    XMLElement * lastTopVisibleEntry = doc.NewElement( WriteXmlFile::LASTTOPVISIBLEENTRY ); // hangs off Group
    lastTopVisibleEntry->SetText(UUID::EMPTYUUIDSTRING);
    group->InsertEndChild( lastTopVisibleEntry );

	//doc.SaveFile( filePath.toLocal8Bit().data() );
    XMLPrinter printer;
    doc.Accept( &printer );
    QString qStr(printer.CStr());
    
    return qStr;
}