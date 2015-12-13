#include "filesystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QVariant>
#include <vector>

#include "readxmlfile.h"
#include "hashedblockstream.h"
#include "compositekey.h"
#include "sha256.h"
#include "salsa20.h"
#include "passwordentry.h"
#include "base64.h"

#include "/home/dan/KeePass3/KeePass3/cryptopp/modes.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/aes.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/filters.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/sha.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/osrng.h"

using namespace std;

#define DEBUG true

char *m_pbMasterSeed; //[32];
char *m_pbCompression;
char *m_cypherUuid; //[16];
char *m_pbTransformSeed;//[32];
char *m_pwDatabaseKeyEncryptionRounds;//[8];
char *m_pbEncryptionIV;//[16];
char *m_pbProtectedStreamKey;//[32];
char *m_pbStreamStartBytes;//[32];
char *m_pbInnerRandomStreamID;//[4];

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

enum DbState : byte {
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
vector<TreeNode*> dataTree;
vector<TreeNode*> current;

Filesystem::Filesystem(QObject *parent) :
    QObject(parent)
{
}

Filesystem::~Filesystem() {
}

QString Filesystem::decryptPassword(QString encryptedPassword)
{
    Base64 base64;
    vector<char> plainEncrypted = base64.base64_decode(encryptedPassword.toStdString());

    SHA256 sha256;
    vector<char> vStreamKey;
    for(int i = 0; i<32;i++) {
        vStreamKey.push_back(m_pbProtectedStreamKey[i]);
    }

    vector<char> vStreamKeyHash = sha256.computeHash(vStreamKey);

    //m_pbInnerRandomStreamID identifies the crypto for encrypted passwords, we only support salsa20
    Salsa20* salsa = new Salsa20(vStreamKeyHash, m_pbIVSalsa);
    byte* bytes = salsa->decrypt(plainEncrypted);

    QString str;
    for(int i=0;i<plainEncrypted.size();i++) {
        str[i] = bytes[i];
    }

    delete salsa;

    return str;
}

QString Filesystem::reloadBranch(QString uuid, int entryType)
{
    QString retVal;
    TreeNode* parent = 0;

    if(entryType == Group) {
        // find branch that holds this uuid and reload it into the model
        if(getMyBranch(uuid, dataTree)) {
            model->removeRows(0, model->rowCount());
            for(int i=0;i<current.size();i++) {
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
            for(int i=0;i<current.size();i++) {
                model->addPasswordEntry(current[i]->passwordEntry());
                if(parent == 0) parent = current[i]->parent();
                if(parent != 0) retVal = parent->passwordEntry().uuid();
            }
        }
    }

    return retVal;
}

void Filesystem::selectBranch(QString uuid)
{
    if(getChildBranch(uuid, dataTree)) {
        model->removeRows(0, model->rowCount());
        for(int i=0;i<current.size();i++) {
            model->addPasswordEntry(current[i]->passwordEntry());
        }
    }
}

bool Filesystem::getChildBranch(QString uuid, vector<TreeNode*> currentBranch)
{
    TreeNode* node;
    for(int i=0;i<currentBranch.size();i++) {
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

bool Filesystem::getMyBranch(QString uuid, vector<TreeNode*> currentBranch)
{
    TreeNode* node;
    for(int i=0;i<currentBranch.size();i++) {
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

PasswordEntryModel* Filesystem::createModel()
{
    model = new PasswordEntryModel();    

    return model;
}

void Filesystem::closeFile() {
    if(m_dbState != open) {
        return;
    }

    // Close the file by clearing all memory items
    // This will mean we have to push the memory items up to member variables
    model->removeRows(0, model->rowCount());
    dataTree.clear();
    m_dbState = closed;
}

void Filesystem::openFile(QString url, QString password) {

    if(m_dbState == open) {
        closeFile();
    }

    std::ifstream file;
    std::streampos size;
    char * memblock;        
    const char * c = url.toStdString().c_str();

    if(!fileExists(c)) {
        emit error("File does not exist");
        return;
    }

    file.open(c, std::ios::in | std::ios::binary| std::ios::ate);
    size = file.tellg();
    memblock = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(memblock, size);
    file.close();

    uint uSig1 = 0, uSig2 = 0, uVersion = 0;  
    uSig1 = loadByte(memblock, 0);
    uSig2 = loadByte(memblock, 4);

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

    uVersion = loadByte(memblock, 8);
    if((uVersion & FileVersionCriticalMask) > (FileVersion32 & FileVersionCriticalMask))
    {
        emit error("Unsupported file version detected");
        return;
    }

    uint offset = 12;
    bool endOfHeaderReached = false;
    while(true)
    {
        // Add try catch to here
        try {
        uint bytesRead = readHeaderField(memblock, offset, &endOfHeaderReached);

        offset += bytesRead;

        if(endOfHeaderReached) {
            break;
        }       
        }
        catch(std::exception &e) {
            emit error("Error parsing database");
            return;
        }
    }

    // Create a SHA256 hash of the header data
    char header[offset];
    for(uint i = 0; i<=offset; i++) {
        header[i] = memblock[i];
    }

    // Not sure yet what this is used for, store it and move on
    // In KeePass its a byte here a std::string
    //std::string hash = generateSHA256Hash(header, offset);
    vector<char> v(header, header + sizeof header / sizeof header[0]);
    SHA256 sha256;
    vector<char> hash = sha256.computeHash(v);

    assert(m_pbMasterSeed != NULL);
    if(m_pbMasterSeed == NULL) {
        emit error("Invalid master seed length");
    }

    // Generate master key
    std::string stringKey = password.toStdString();
    const byte * key = reinterpret_cast<const byte*>(stringKey.c_str());

    vector<char> vKey;
    for(int i = 0; i<password.size();i++) {
        vKey.push_back(key[i]);
    }

    vector<char> vKeySeed;
    for(int i = 0; i<32;i++) { //TODO: Capture the size of the seed from the header don't assume 32        
        vKeySeed.push_back(m_pbTransformSeed[i]);
    }

    uint uNumRounds = readBytes(m_pwDatabaseKeyEncryptionRounds, 0, 8);
    CompositeKey* cmpKey = new CompositeKey(vKey);
    vector<char> pKey32 = cmpKey->generateKey32(vKeySeed, uNumRounds);    
    delete cmpKey;
    cmpKey = 0;
    vector<char> masterSeed;
    for(int i=0; i<32;i++) {
        masterSeed.push_back(m_pbMasterSeed[i]);
    }

    vector<char> ms;
    ms.reserve( vKeySeed.size() + pKey32.size() ); // preallocate memory
    ms.insert( ms.end(), masterSeed.begin(), masterSeed.end() );
    ms.insert( ms.end(), pKey32.begin(), pKey32.end() );

    vector<char> aesKey = sha256.computeHash(ms);

    if(aesKey.size() != 32) {
        emit error("FinalKey creation failed");
    }

    byte pbAesKey[32];
    for(uint i=0;i<32;i++){
        pbAesKey[i] = aesKey[i];
    }

    uint contentSize = size-offset;
    byte pbFileContent[contentSize];
    for(uint i=0;i<contentSize;i++) {
        pbFileContent[i] = memblock[i+offset];
    }

    string recovered;
    try
    {        
        CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
        d.SetKeyWithIV( pbAesKey, 32, (byte*)m_pbEncryptionIV );

        // The StreamTransformationFilter removes
        //  padding as required.
        CryptoPP::ArraySource ss( pbFileContent, contentSize, true,
            new CryptoPP::StreamTransformationFilter( d,
                new CryptoPP::StringSink( recovered )
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (CryptoPP::InvalidCiphertext &ex)
    {
        emit error("Could not decryt the file");
    }

    vector<char> startBytes;
    for(int i=0;i<32;i++) {
        startBytes.push_back(recovered[i]);
    }

    if(m_pbStreamStartBytes == NULL) {
        emit error("Invalid data read");
    }

    for(int iStart = 0; iStart<32;iStart++) {
        if(startBytes[iStart] != m_pbStreamStartBytes[iStart]) {
            emit error("Invalid composite key");
        }
    }

    if(m_pbProtectedStreamKey == NULL) {
        emit error("Invalid protected stream key");
    }

    // Create sha256 hash of the m_pbProtectedStreamKey
    /*vector<char> vStreamKey;
    for(int i = 0; i<32;i++) {
        vStreamKey.push_back(m_pbProtectedStreamKey[i]);
    }

    vector<char> vStreamKeyHash = sha256.computeHash(vStreamKey);

    char pbIV[] = { 0xE8, 0x30, 0x09, 0x4B,
                  0x97, 0x20, 0x5D, 0x2A }; // Unique constant

    vector<char> vIv (pbIV, pbIV + sizeof pbIV / sizeof pbIV[0]);
    Salsa20 *salsa20 = new Salsa20(vStreamKeyHash, vIv);

    // Salsa20 it, Salsa20 What?!
*/
    vector<char> payload;
    uint recoveredOffset = 32;
    for(int i=recoveredOffset;i<recovered.size(); i++) {
        payload.push_back(recovered[i]);
    }

    assert(recovered.size() > 0);
    vector<char> read;
    assert(read.size() == 0);
    HashedBlockStream *hashedStream = new HashedBlockStream(payload, false, 0, true);

    // Read the payload in blocks of 512 bytes (this could be anything but it
    // gives us a chance to give user feedback)
    int readBytes = 0;
    int i=0;
    do
     {
        readBytes = hashedStream->Read(&read, (i*4096), 4096);
        i++;
        // Update the GUI with some feedback
    } while(readBytes > 0);

    delete hashedStream;
    hashedStream = 0;
    assert (hashedStream == 0);
    assert(read.size() > 0);

    // If we got here we will have the full xml file in read.
    // Only do this if we're debugging
    if(DEBUG) {
       ofstream f("debug.txt");
       for(vector<char>::const_iterator i = read.begin(); i != read.end(); ++i) {
            f << *i;
       }
    }

    // We have Xml so we need to parse it. My idea is to convert the entire Xml file into c++ objects and then
    // pass them back a level at a time as requested
    const char* xml = read.data();
    assert(read.size() > 0);
    ReadXmlFile *readXml = new ReadXmlFile(xml, read.size());
    dataTree = readXml->GetTopGroup();

    for(int i=0;i<dataTree.size();i++) {
        model->addPasswordEntry(dataTree[i]->passwordEntry());
    }

    m_dbState = open;

    emit success(m_vl);

}

// Returns true if the file exists else false
bool Filesystem::fileExists(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

// Returns the number of bytes read so that we can keep track of our offset.
// If -1 returned then we are finished
uint Filesystem::readHeaderField(char* memblock, int offset, bool* endOfHeaderReached)
{    
    if(memblock == NULL)
    {
        throw std::exception();
    }

    char btFieldID = memblock[offset++];
    ushort uSize = (ushort)readBytes(memblock, offset, 2);
    offset = offset+2;

    char pbData[uSize];
    if(uSize > 0)
    {
        for(int i = 0; i<uSize; i++) {
            pbData[i] = memblock[(offset+i)];
        }
    }

    uint uCompression;
    KdbxHeaderFieldID kdbID = (KdbxHeaderFieldID)btFieldID;
    switch(kdbID)
    {
        case EndOfHeader:
            *endOfHeaderReached = true; // Returning 0 indicates end of header (no bytes read). Figure out how to pass an out parameter so we can check that for an end flag
            break;

        case CipherID:
            // Only aes cipher supported, check and throw exception if not correct
            m_cypherUuid = new char[uSize];
            copy(pbData, pbData + uSize, m_cypherUuid);
            if(!equal(m_uuidAes, m_cypherUuid, uSize)) {
                throw std::exception();
            }
            break;

        case CompressionFlags:
            // Compression not supported (check pbData is 0 and throw exception if not)
            // Put in version 2 if version 1 ever gets off the ground
            m_pbCompression = new char[uSize];
            copy(pbData, pbData + uSize, m_pbCompression);
            uCompression = loadByte(m_pbCompression, 0);
            if(uCompression != 0) {
                throw new std::exception();
            }
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

    return (uSize + 3);
}

uint Filesystem::readBytes(char* memblock, int offset, uint size)
{
    uint result = 0;
    byte tmp[size];

    for(uint i = 0; i<size; i++) {
        tmp[i] = memblock[(i+offset)];
    }

    for(int i = (size - 1); i>=0; i--) {
        result = (result << 8) + (unsigned char)tmp[i];
    }

    return result;
}

uint Filesystem::loadByte(char* memblock, int offset)
{
    return readBytes(memblock, offset, 4);
}

bool Filesystem::equal(char* type1, char* type2, uint size) {
    for(uint i = 0; i<size;i++) {
        if(type1[i] != type2[i]) {
            return false;
        }
    }

    return true;
}
