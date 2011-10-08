/*
 * Dibbler - a portable DHCPv6
 *
 * author:  Grzgorz Pluto
 * changes: Tomasz Mrugalski
 *
 * released under GNU GPL v2 only licence
 *
 */

#include "SrvMsgReconfigure.h"
#include "Logger.h"
#include "SrvOptOptionRequest.h"
#include "SrvOptClientIdentifier.h"
#include "SrvOptIA_NA.h"
#include "SrvOptTA.h"
#include "SrvOptServerUnicast.h"
#include "SrvOptStatusCode.h"
#include "SrvOptServerIdentifier.h"
#include "OptReconfigureMsg.h"
#include "Opt.h"
#include "SrvOptPreference.h"
#include "SrvOptDNSServers.h"
#include "SrvOptNTPServers.h"
#include "SrvOptTimeZone.h"
#include "SrvOptDomainName.h"
#include "SrvOptFQDN.h"
#include "SrvOptIA_PD.h"
#include "OptReconfigureMsg.h"
#include "Logger.h"

TSrvMsgReconfigure::TSrvMsgReconfigure(int iface, SPtr<TIPv6Addr> clientAddr,
                                       SPtr<TIPv6Addr> ia ,int msgType,
                                       SPtr<TDUID> ptrDUID)
    :TSrvMsg(iface, clientAddr, RECONFIGURE_MSG, 0 )
{
    ClientDUID=ptrDUID;

    // status code is not necessary in Reconfigure
    //appendStatusCode();

    //appendAuthenticationOption(ClientDUID);
    // append serverID, preference and possibly unicast
    //appendMandatoryOptions(ORO);
    
    //if client requested parameters and policy doesn't forbid from answering
    //appendRequestedOptions(ClientDUID, clientAddr, iface, ORO);

    // include our DUID (Server ID)
    SPtr<TSrvOptServerIdentifier> ptrSrvID;
    ptrSrvID = new TSrvOptServerIdentifier(SrvCfgMgr().getDUID(),this);
    Options.push_back((Ptr*)ptrSrvID);

    // include his DUID (Client ID)
    SPtr<TOptDUID> clientDuid = new TOptDUID(OPTION_CLIENTID, ptrDUID, this);
    Options.push_back( (Ptr*)clientDuid);

    // include Reconfigure Message Options
    Options.push_back(new TOptReconfigureMsg(RENEW_MSG, this) );

    // SPtr<TSrvOptReconfigureMsg> ptrSrvReconf;
    // ptrSrvReconf = new TSrvOptReconfigureMsg(5,this);
    // Options.push_back((Ptr*)ptrSrvReconf);

    pkt = new char[this->getSize()];
    send();
    return;
}

bool TSrvMsgReconfigure::check() {
    // this should never happen
    return true;
}

TSrvMsgReconfigure::~TSrvMsgReconfigure() {
}

unsigned long TSrvMsgReconfigure::getTimeout() {
    return 0;
}
void TSrvMsgReconfigure::doDuties() {
    IsDone = true;
}

string TSrvMsgReconfigure::getName() {
    return "RECONFIGURE";
}
