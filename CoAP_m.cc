//
// Generated file, do not edit! Created by nedtool 5.1 from CoAP.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "CoAP_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("packetType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("packetType"));
    e->insert(CONFIRMABLE, "CONFIRMABLE");
    e->insert(NON_CONFIRMABLE, "NON_CONFIRMABLE");
    e->insert(ACK, "ACK");
    e->insert(RESET, "RESET");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("packetCode");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("packetCode"));
    e->insert(REQUEST, "REQUEST");
    e->insert(SUCCESS, "SUCCESS");
    e->insert(CLIENT_ERROR, "CLIENT_ERROR");
    e->insert(SERVER_ERROR, "SERVER_ERROR");
)

Register_Class(CoAP)

CoAP::CoAP(const char *name, short kind) : ::IoTPacket(name,kind)
{
    this->type = 0;
    this->code = 0;
    this->hasToken = false;
    this->hasOptions = false;
    this->hasPayload = false;
}

CoAP::CoAP(const CoAP& other) : ::IoTPacket(other)
{
    copy(other);
}

CoAP::~CoAP()
{
}

CoAP& CoAP::operator=(const CoAP& other)
{
    if (this==&other) return *this;
    ::IoTPacket::operator=(other);
    copy(other);
    return *this;
}

void CoAP::copy(const CoAP& other)
{
    this->type = other.type;
    this->code = other.code;
    this->hasToken = other.hasToken;
    this->hasOptions = other.hasOptions;
    this->hasPayload = other.hasPayload;
    this->payload = other.payload;
}

void CoAP::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::IoTPacket::parsimPack(b);
    doParsimPacking(b,this->type);
    doParsimPacking(b,this->code);
    doParsimPacking(b,this->hasToken);
    doParsimPacking(b,this->hasOptions);
    doParsimPacking(b,this->hasPayload);
    doParsimPacking(b,this->payload);
}

void CoAP::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::IoTPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->type);
    doParsimUnpacking(b,this->code);
    doParsimUnpacking(b,this->hasToken);
    doParsimUnpacking(b,this->hasOptions);
    doParsimUnpacking(b,this->hasPayload);
    doParsimUnpacking(b,this->payload);
}

int CoAP::getType() const
{
    return this->type;
}

void CoAP::setType(int type)
{
    this->type = type;
}

int CoAP::getCode() const
{
    return this->code;
}

void CoAP::setCode(int code)
{
    this->code = code;
}

bool CoAP::getHasToken() const
{
    return this->hasToken;
}

void CoAP::setHasToken(bool hasToken)
{
    this->hasToken = hasToken;
}

bool CoAP::getHasOptions() const
{
    return this->hasOptions;
}

void CoAP::setHasOptions(bool hasOptions)
{
    this->hasOptions = hasOptions;
}

bool CoAP::getHasPayload() const
{
    return this->hasPayload;
}

void CoAP::setHasPayload(bool hasPayload)
{
    this->hasPayload = hasPayload;
}

const char * CoAP::getPayload() const
{
    return this->payload.c_str();
}

void CoAP::setPayload(const char * payload)
{
    this->payload = payload;
}

class CoAPDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CoAPDescriptor();
    virtual ~CoAPDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CoAPDescriptor)

CoAPDescriptor::CoAPDescriptor() : omnetpp::cClassDescriptor("CoAP", "IoTPacket")
{
    propertynames = nullptr;
}

CoAPDescriptor::~CoAPDescriptor()
{
    delete[] propertynames;
}

bool CoAPDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CoAP *>(obj)!=nullptr;
}

const char **CoAPDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CoAPDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CoAPDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int CoAPDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *CoAPDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "type",
        "code",
        "hasToken",
        "hasOptions",
        "hasPayload",
        "payload",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int CoAPDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "code")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hasToken")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hasOptions")==0) return base+3;
    if (fieldName[0]=='h' && strcmp(fieldName, "hasPayload")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CoAPDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "bool",
        "bool",
        "bool",
        "string",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **CoAPDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        case 1: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *CoAPDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "packetType";
            return nullptr;
        case 1:
            if (!strcmp(propertyname,"enum")) return "packetCode";
            return nullptr;
        default: return nullptr;
    }
}

int CoAPDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CoAP *pp = (CoAP *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CoAPDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CoAP *pp = (CoAP *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CoAPDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CoAP *pp = (CoAP *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getType(), "packetType");
        case 1: return enum2string(pp->getCode(), "packetCode");
        case 2: return bool2string(pp->getHasToken());
        case 3: return bool2string(pp->getHasOptions());
        case 4: return bool2string(pp->getHasPayload());
        case 5: return oppstring2string(pp->getPayload());
        default: return "";
    }
}

bool CoAPDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CoAP *pp = (CoAP *)object; (void)pp;
    switch (field) {
        case 0: pp->setType((packetType)string2enum(value, "packetType")); return true;
        case 1: pp->setCode((packetCode)string2enum(value, "packetCode")); return true;
        case 2: pp->setHasToken(string2bool(value)); return true;
        case 3: pp->setHasOptions(string2bool(value)); return true;
        case 4: pp->setHasPayload(string2bool(value)); return true;
        case 5: pp->setPayload((value)); return true;
        default: return false;
    }
}

const char *CoAPDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CoAPDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CoAP *pp = (CoAP *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


