
/*  © Copyright CERN, 2015. All rights not expressly granted are reserved.

    This file was generated by quasar (https://github.com/quasar-team/quasar/)

    Quasar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public Licence as published by
    the Free Software Foundation, either version 3 of the Licence.
    Quasar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public Licence for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Quasar.  If not, see <http://www.gnu.org/licenses/>.

 */


#ifndef __ASRegs__H__
#define __ASRegs__H__

/* From relevant OPC-UA toolkit ... */
#include <opcua_baseobjecttype.h>
#include <methodhandleuanode.h>

/* From quasar's address-space module ... */
#include <ASCommon.h>
#include <ASDelegatingMethod.h>
#include <ASNodeManager.h>
#include <ASDelegatingVariable.h>
#include <ASSourceVariable.h>

/* From quasar's configuration module ... */
#include <Configuration.hxx>

/* forward declaration */
namespace Device {
class DRegs;
}

namespace AddressSpace
{
class ChangeNotifyingVariable;

//! Fully auto-generated class to represent Regs in the OPC UA AddressSpace
class ASRegs: public OpcUa::BaseObjectType
{
public:
    //! Constructor. Used in Configurator.cpp. You NEVER use it directly.
    ASRegs (
        UaNodeId parentNodeId,
        const UaNodeId& typeNodeId,
        ASNodeManager *nm,
        const Configuration::Regs& config);

    // Note: we get rid off copy ctr and assignment operator, implicitly getting rid of move ctr + oper.
    ASRegs ( ASRegs& other ) = delete;
    ASRegs& operator= ( ASRegs& other ) = delete;

    /*dtor*/
    ~ASRegs ();

    /* setters and getters for variables */
    /* setters and getters for cache-variable "userReg" */
    UaStatus getUserReg (OpcUa_UInt32& out) const;
    UaStatus setUserReg( const OpcUa_UInt32 value, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );
    /* null-setter (possible because nullPolicy=nullAllowed) */
    UaStatus setNullUserReg( OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now());
    /* now the new-style setter */
    UaStatus setUserReg( QuasarNullDataType null, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );

    /* setters and getters for cache-variable "writeRegValue" */
    UaStatus getWriteRegValue (OpcUa_UInt32& out) const;
    UaStatus setWriteRegValue( const OpcUa_UInt32 value, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );
    /* null-setter (possible because nullPolicy=nullAllowed) */
    UaStatus setNullWriteRegValue( OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now());
    /* now the new-style setter */
    UaStatus setWriteRegValue( QuasarNullDataType null, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );

    /* setters and getters for cache-variable "FPGAuptime" */
    UaStatus getFPGAuptime (OpcUa_Double& out) const;
    UaStatus setFPGAuptime( const OpcUa_Double value, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );
    /* null-setter (possible because nullPolicy=nullAllowed) */
    UaStatus setNullFPGAuptime( OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now());
    /* now the new-style setter */
    UaStatus setFPGAuptime( QuasarNullDataType null, OpcUa_StatusCode statusCode, const UaDateTime& srcTime= UaDateTime::now() );


    /* delegators for cachevariables  */
    UaStatus writeWriteRegValue( Session* pSession, const UaDataValue& dataValue, OpcUa_Boolean checkAccessLevel = OpcUa_True );

    /* delegators for methods */

    /* device logic links (if device logic is present per this class) */
    void linkDevice( Device::DRegs* deviceLink);
    void unlinkDevice ();
    Device::DRegs* getDeviceLink () const {
        return m_deviceLink;
    }

    /* OPC UA Type Information provider for this class. */
    virtual UaNodeId typeDefinitionId () const {
        return m_typeNodeId;
    }


private:
    UaNodeId m_typeNodeId;
    const UaNodeId m_effectiveParentNodeIdForChildren;

    UaString fixChildNameWhenSingleNodeClass(
        const std::string& nameByDesign,
        const std::string& nameByConfig
    ) const;

    void createCacheVariables(
        ASNodeManager* nm,
        const Configuration::Regs& config);
    void initializeArrayCacheVariablesFromConfiguration(
        ASNodeManager* nm,
        const Configuration::Regs& config);
    void createSourceVariables(
        ASNodeManager* nm,
        const Configuration::Regs& config);
    void createMethods(
        ASNodeManager* nm,
        const Configuration::Regs& config);
    void createPropertiesFromConfigEntries(
        ASNodeManager* nm,
        const Configuration::Regs& config);

    /* Variables */
    ChangeNotifyingVariable* m_userReg;
    ChangeNotifyingVariable* m_writeRegValue;
    ChangeNotifyingVariable* m_FPGAuptime;


    /* Methods */

    /* Device Logic link (if requested) */
    Device::DRegs* m_deviceLink;

};


}

#endif // __ASRegs__H__