/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
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

#include "database.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QVariant>
#include <QString>
#include <vector>

#include "aes.h"
#include "arrayextensions.h"
#include "readxmlfile.h"
#include "hashedblockstream.h"
#include "compositekey.h"
#include "sha256.h"
#include "salsa20.h"
#include "passwordentry.h"
#include "base64.h"
#include "readkeyfile.h"
#include "writexmlfile.h"
#include "filehandler.h"
#include "bytestream.h"
#include "../ziplib/GZipHelper.h"

using namespace std;

#define FINALKEYSIZE 32
#define MASTERSEEDSIZE 32
#define CYPHERUUIDSIZE 16
#define TRANSFORMSEEDSIZE 32
#define ENCRYPTIONROUNDSSIZE 8
#define ENCRYPTIONIVSIZE 16
#define PROTECTEDSTREAMKEYSIZE 32
#define STREAMSTARTBYTESSIZE 32
#define INNERRANDOMSEEDSIZE 4

char m_pbIVSalsa[] = { 0xE8, 0x30, 0x09, 0x4B,
                    0x97, 0x20, 0x5D, 0x2A };

char m_uuidAes[] = {
                        0x31, 0xC1, 0xF2, 0xE6, 0xBF, 0x71, 0x43, 0x50,
                        0xBE, 0x58, 0x05, 0x21, 0x6A, 0xFC, 0x5A, 0xFF };

enum KdbxHeaderFieldID : byte
{
    EndOfHeader = 0,
    Comment = 1,
    CipherID = 2,
    CompressionFlags = 3,
    MasterSeed = 4,
    TransformSeed = 5,
    TransformRounds = 6,
    EncryptionIV = 7,
    ProtectedStreamKey = 8,
    StreamStartBytes = 9,
    InnerRandomStreamID = 10
};

enum DbState {
    closed = 0,
    open = 1
};


DbState m_dbState;
const uint FileSignature1 = 0x9AA2D903;
const uint FileSignatureOld1 = 0x9AA2D903;
const uint FileSignature2 = 0xB54BFB67;
const uint FileSignatureOld2 = 0xB54BFB65;
const uint FileSignaturePreRelease1 = 0x9AA2D903;
const uint FileSignaturePreRelease2 = 0xB54BFB66;
const uint FileVersionCriticalMask = 0xFFFF0000;
const uint FileVersion32 = 0x00030001;
PasswordEntryModel* model;

///
/// \brief Database::Database
/// \param parent
///
Database::Database(QObject *parent) :
    QObject(parent)
{
}

///
/// \brief Database::~Database
///
Database::~Database() {
}

void Database::createNewDatabase(QString filePath) {
    // Create the XML (the file should also be SALSA'ed)
    /*QString db = WriteXmlFile::CreateNewDatabase(filePath);
    
    //GZIP the string
    char * cpy = new char[db.size()];
    strncpy(cpy, db.toStdString().c_str(), db.size()); 
    CA2GZIP gzip(cpy, db.size());
    LPGZIP pgzip = gzip.pgzip; // zipped data pointer
    
    // Generate encryption aes keys and IV
    // Encrypt it
    //pgzip.

    //Save it
    */
}

///
/// \brief Database::deleteFile
/// \param filePath
///
void Database::deleteFile(QString filePath) {
    FileHandler::deleteFile(filePath);
}

///
/// \brief Database::loadHome
///
void Database::loadHome() {
    model->removeRows(0, model->rowCount());
    for(uint i=0;i<dataTree.size();i++) {
        model->addPasswordEntry(dataTree[i]->passwordEntry());
    }
}

///
/// \brief Database::search
/// \param name
///
void Database::search(QString name) {
    foundAny = false;
    searchInternal(name, dataTree);
}

///
/// \brief Database::searchInternal
/// \param name
/// \param node
///
void Database::searchInternal(QString name, vector<TreeNode*> node) {
    for(uint i=0;i<node.size();i++) {
        QString strTitle = node[i]->passwordEntry().title();
        if(strTitle.indexOf(name, 0, Qt::CaseInsensitive) >= 0) {
            if(!foundAny) {
                model->removeRows(0, model->rowCount());
            }
            model->addPasswordEntry(node[i]->passwordEntry());
            foundAny = true;
        }

        if(node[i]->next().size() > 0 ) {
            searchInternal(name, node[i]->next());
        }
    }
}

///
/// \brief Database::reloadBranch
/// \param uuid
/// \param entryType
/// \return
///
QString Database::reloadBranch(QString uuid, int entryType)
{
    QString retVal;
    TreeNode* parent = 0;

    if(entryType == Group) {
        // find branch that holds this uuid and reload it into the model
        if(getMyBranch(uuid, dataTree)) {
            model->removeRows(0, model->rowCount());
            for(uint i=0;i<current.size();i++) {
                model->addPasswordEntry(current[i]->passwordEntry());
                if(parent == 0) parent = current[i]->parent();
                if(parent != 0) retVal = parent->passwordEntry().uuid();
            }
        }
    }

    else if( entryType == Entry) {
        // find branch that holds this uuid and reload it into the model
        if(getMyBranch(uuid, dataTree)) {
            model->removeRows(0, model->rowCount());
            for(uint i=0;i<current.size();i++) {
                model->addPasswordEntry(current[i]->passwordEntry());
                if(parent == 0) parent = current[i]->parent();
                if(parent != 0) retVal = parent->passwordEntry().uuid();
            }
        }
    }

    return retVal;
}

///
/// \brief Database::selectBranch
/// \param uuid
///
void Database::selectBranch(QString uuid)
{
    if(getChildBranch(uuid, dataTree)) {
        model->removeRows(0, model->rowCount());
        for(uint i=0;i<current.size();i++) {
            model->addPasswordEntry(current[i]->passwordEntry());
        }
    }
}

///
/// \brief Database::getChildBranch
/// \param uuid
/// \param currentBranch
/// \return
///
bool Database::getChildBranch(QString uuid, vector<TreeNode*> currentBranch)
{
    TreeNode* node;
    for(uint i=0;i<currentBranch.size();i++) {
        node = currentBranch[i];
        if(node->passwordEntry().uuid() == uuid) {
            current = node->next();
            return true;
        }

        else if(node->next().size() > 0) {
            if(getChildBranch(uuid, node->next())) {
                return true;
            }
        }
    }

    return false;
}

///
/// \brief Database::getMyBranch
/// \param uuid
/// \param currentBranch
/// \return
///
bool Database::getMyBranch(QString uuid, vector<TreeNode*> currentBranch)
{
    TreeNode* node;
    for(uint i=0;i<currentBranch.size();i++) {
        node = currentBranch[i];
        if(node->passwordEntry().uuid() == uuid) {
            current = currentBranch;
            return true;
        }

        else if(node->next().size() > 0) {
            if(getMyBranch(uuid, node->next())) {
                return true;
            }
        }
    }

    return false;
}

///
/// \brief Database::createModel
/// \return
///
PasswordEntryModel* Database::createModel()
{
    model = new PasswordEntryModel();

    return model;
}

///
/// \brief Database::closeFile
///
void Database::closeFile() {
    if(m_dbState != open) {
        return;
    }

    // Close the file by clearing all memory items
    // This will mean we have to push the memory items up to member variables
    model->removeRows(0, model->rowCount());
    dataTree.clear();
    current.clear();
    m_dbState = closed;
}

///
/// \brief Database::openFile
/// \param url
/// \param password
/// \param passKey
///
void Database::openFile(QString url, QString password, QString passKey) {

    if(m_dbState == open) {
        closeFile();
    }

    FileHandler fileHandler;
    Aes aes;
    ArrayExtensions ae;
    std::streampos size, passKeySize;

    char* memblock = fileHandler.readFile(url, size);
    if(memblock == 0) {
        emit error("Database does not exist");
        return;
    }

    ByteStream *byteStream = new ByteStream(memblock, size);

    char* passKeyMemblock;
    bool hasKeyFile = false;
    if(strcmp(passKey.toStdString().c_str(), "") != 0) {
        passKeyMemblock = fileHandler.readFile(passKey, passKeySize);
        if(passKeyMemblock == 0) {
            emit error("Key file does not exist");
            return;
        }

        hasKeyFile = true;
    }

    uint uSig1 = 0, uSig2 = 0, uVersion = 0;
    uSig1 = byteStream->ReadByte();
    uSig2 = byteStream->ReadByte();

    assert(uSig1 != FileSignatureOld1 || uSig2 != FileSignatureOld2);
    if (uSig1 == FileSignatureOld1 && uSig2 == FileSignatureOld2) {
        emit error("Unsupported database format detected");
        return;
    }

    assert(uSig1 == FileSignature1 && uSig2 == FileSignature2);
    if((uSig1 == FileSignature1) && (uSig2 == FileSignature2)) {}
    else if((uSig1 == FileSignaturePreRelease1) && (uSig2 == FileSignaturePreRelease2)) {}
    else {
        emit error("Unknown file signature detected");
        return;
    }

    uVersion = byteStream->ReadByte();
    if((uVersion & FileVersionCriticalMask) > (FileVersion32 & FileVersionCriticalMask))
    {
        emit error("Unsupported file version detected");
        return;
    }

    bool endOfHeaderReached = false;
    bool readError = false;
    while(true)
    {
        try {
            readHeaderField(byteStream, &endOfHeaderReached, &readError);

            if(readError) {
                return;
            }

            if(endOfHeaderReached) {
                break;
            }
        }
        catch(const std::exception &e) {
            emit error("Error parsing database");
            return;
        }
    }

    // Create a SHA256 hash of the header data
    //char header[offset];
    //for(uint i = 0; i<=offset; i++) {
        //header[i] = memblock[i];
    //    header[i] = byteStream->Read();
    //}

    // Not sure yet what this is used for, store it and move on
    // In KeePass its a byte here a std::string
    //std::string hash = generateSHA256Hash(header, offset);
    //vector<char> v(header, header + sizeof header / sizeof header[0]);
    SHA256 sha256;
    //vector<char> hash = sha256.computeHash(v);

    assert(m_pbMasterSeed != NULL);
    if(m_pbMasterSeed == NULL) {
        emit error("Invalid master seed length");
        return;
    }

    // Generate master key
    // Read keyfile if necessary
    ReadKeyFile readKeyFile;
    vector<char> vKeyFileData;
    if(hasKeyFile) {
        vKeyFileData = readKeyFile.read(passKeyMemblock, (int)passKeySize);
    }

	// 'toStdString' encodes the password string in UTF-8
    string stringKey = password.toStdString();
    const byte * key = reinterpret_cast<const byte*>(stringKey.c_str());
    
	// Some UTF-8 characters are represented with more than one byte.
	// Only 'stringKey.length()' returns the length of the string in bytes.
	// Therefore use the length of 'stringKey' and not the length of 'password'
	vector<char> vKey = ae.toVector((char*)key, (uint)stringKey.length());
    vector<char> vKeySeed = ae.toVector(m_pbTransformSeed, TRANSFORMSEEDSIZE);

    uint uNumRounds = ByteStream::ReadByte(m_pwDatabaseKeyEncryptionRounds);

    CompositeKey* cmpKey = new CompositeKey(vKey, vKeyFileData);
    vector<char> pKey32 = cmpKey->generateKey32(vKeySeed, uNumRounds);
    delete cmpKey;
    cmpKey = 0;

    vector<char> masterSeed = ae.toVector(m_pbMasterSeed, MASTERSEEDSIZE);
    vector<char> ms;
    ms.reserve( vKeySeed.size() + pKey32.size() ); // preallocate memory
    ms.insert( ms.end(), masterSeed.begin(), masterSeed.end() );
    ms.insert( ms.end(), pKey32.begin(), pKey32.end() );
    vector<char> aesKey = sha256.computeHash(ms);

    if(aesKey.size() != FINALKEYSIZE) {
        emit error("FinalKey creation failed");
        return;
    }

    byte pbAesKey[FINALKEYSIZE];
    for(uint i=0;i<FINALKEYSIZE;i++){
        pbAesKey[i] = aesKey[i];
    }

    uint contentSize = size-byteStream->GetPosition();
    byte pbFileContent[contentSize];
    for(uint i=0;i<contentSize;i++) {
        pbFileContent[i] = byteStream->Read();
    }

    delete byteStream;
    byteStream = NULL;

    string recovered;
    try {
         recovered = aes.decrypt(pbAesKey, FINALKEYSIZE, (byte*)m_pbEncryptionIV, (byte*)pbFileContent, contentSize);
    } catch (CryptoPP::InvalidCiphertext &ex) {
        emit error("Invalid composite key");
        return;
    }

    vector<char> startBytes = ae.toVector((char*)recovered.c_str(), STREAMSTARTBYTESSIZE);
    if(m_pbStreamStartBytes == NULL) {
        emit error("Invalid data read");
        return;
    }

    for(int iStart = 0; iStart<STREAMSTARTBYTESSIZE;iStart++) {
        if(startBytes[iStart] != m_pbStreamStartBytes[iStart]) {
            emit error("Invalid composite key");
            return;
        }
    }

    if(m_pbProtectedStreamKey == NULL) {
        emit error("Invalid protected stream key");
        return;
    }

    vector<char> vStreamKey = ae.toVector(m_pbProtectedStreamKey, PROTECTEDSTREAMKEYSIZE);
    vector<char> vStreamKeyHash = sha256.computeHash(vStreamKey);
    salsa = new Salsa20(vStreamKeyHash, m_pbIVSalsa);

    vector<char> payload;
    uint recoveredOffset = 32;

    for(uint i=recoveredOffset;i<recovered.size(); i++) {
        payload.push_back(recovered[i]);
    }

    assert(recovered.size() > 0);
    vector<char> read;
    assert(read.size() == 0);

    try {
        // Now lets read the payload
        readPayload(&read, payload);

        char *inter = new char[read.size()];
        for(uint i = 0;i<read.size();i++) {
            inter[i] = read[i];
        }

        // Are we gzipped, if so lets unzip:
        if(uCompression == 1) { // We are compressed
            // payload == the compressed buffer
            CGZIP2A a((LPGZIP)inter, read.size());
            read.clear();

            for(uint i=0;i<a.Length; i++) {
                read.push_back(a.psz[i]); // push uncompressed data back into read vector
            }
        }

    } catch(exception &ex) {
        emit error("Could not read payload (incorrect composite key?)");
        return;
    }

    // We have Xml so we need to parse it. My idea is to convert the entire Xml file into c++ objects and then
    // pass them back a level at a time as requested
    // At this point the entire DB is decrypted in memory (is there any way to harden this?)
    const char* xml = read.data(); // Can I just pass the read vector to the ReadXmlFile class instead of creating another pointer here?
    assert(read.size() > 0);
    ReadXmlFile *readXml = new ReadXmlFile(xml, read.size(), salsa);
    dataTree = readXml->GetTopGroup();
    loadHome();

    ArrayExtensions::Reset(payload); // This should be reset when the HashedBlockStream is disposed (we need to be passing it by ref maybe?)
    ArrayExtensions::Reset(read);

    delete readXml;
    readXml = 0;

    m_dbState = open;

    emit success();

    return;
}

///
/// \brief Database::readPayload
/// \param read
/// \param payload
///
void Database::readPayload(vector<char>* read, vector<char> payload) {

    HashedBlockStream *hashedStream = new HashedBlockStream(payload, false, 0, true);

    int readBytes = 0;
    int i=0;
    int sz = 1024;
    do
     {
        readBytes = hashedStream->Read(read, (i*sz), sz);
        i++;
    } while(readBytes > 0);

    delete hashedStream;
    hashedStream = 0;
    assert (hashedStream == 0);
}

///
/// \brief Database::readHeaderField
///        Read the header information from the stream
/// \param byteStream
/// \param endOfHeaderReached
/// \param readError
///
void Database::readHeaderField(ByteStream* byteStream, bool* endOfHeaderReached, bool *readError)
{
    char btFieldID = byteStream->Read();
    ushort uSize = byteStream->ReadShort();

    char pbData[uSize];
    if(uSize > 0)
    {
        for(int i = 0; i<uSize; i++) {
            pbData[i] = byteStream->Read();
        }
    }

    KdbxHeaderFieldID kdbID = (KdbxHeaderFieldID)btFieldID;
    switch(kdbID)
    {
        case EndOfHeader:
            *endOfHeaderReached = true;
            break;

        case CipherID:
            // Only aes cipher supported, check and throw exception if not correct
            m_cypherUuid = new char[uSize];
            copy(pbData, pbData + uSize, m_cypherUuid);
            if(!ArrayExtensions::Equal(m_uuidAes, m_cypherUuid, uSize)) {
                throw std::exception();
            }
            break;

        case CompressionFlags:
            m_pbCompression = new char[uSize];
            copy(pbData, pbData + uSize, m_pbCompression);
            uCompression = ByteStream::ReadByte(m_pbCompression);
            break;

        case MasterSeed:
           // Not sure what this is doing so, move on and come back
           m_pbMasterSeed = new char[uSize];
           copy(pbData, pbData + uSize, m_pbMasterSeed);
           // CryptoRandom.Instance.AddEntropy(pbData);
            break;

        case TransformSeed:
            // Not sure what this is doing so, move on and come back
            m_pbTransformSeed = new char[uSize];
            copy(pbData, pbData + uSize, m_pbTransformSeed);
           // CryptoRandom.Instance.AddEntropy(pbData);
            break;

        case TransformRounds:
            m_pwDatabaseKeyEncryptionRounds  = new char[uSize];
            copy(pbData, pbData + uSize, m_pwDatabaseKeyEncryptionRounds);
            break;

        case EncryptionIV:
            m_pbEncryptionIV  = new char[uSize];
            copy(pbData, pbData + uSize, m_pbEncryptionIV);
            break;

        case ProtectedStreamKey:
            // Not sure what this is doing so, move on and come back
           m_pbProtectedStreamKey = new char[uSize];
           copy(pbData, pbData + uSize, m_pbProtectedStreamKey);
           // CryptoRandom.Instance.AddEntropy(pbData);
            break;

        case StreamStartBytes:
            m_pbStreamStartBytes = new char[uSize];
            copy(pbData, pbData + uSize, m_pbStreamStartBytes);
            break;

        case InnerRandomStreamID:
            m_pbInnerRandomStreamID = new char[uSize];
            copy(pbData, pbData + uSize, m_pbInnerRandomStreamID);
           // SetInnerRandomStreamID(pbData);
            break;

        default:
            throw std::exception();
            break;
    }

    return;
}
