#include <ace/Version.h>
#include <ace/Synch_Traits.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Object_Manager.h>
#include <ace/Sig_Handler.h>

#include "ClientSocket.hpp"
#include "ClientSessionMgr.hpp"
#include "Config.hpp"
#include "Configuration.hpp"
#include "Log.hpp"
#include "NetworkMgr.hpp"
#include "Server.hpp"

#ifndef SERVER_CONFIG
#define SERVER_CONFIG "server.conf"
#endif /* SERVER_CONFIG */

#define SERVER_PORT 39235

extern int main(int argc, char **argv)
{
    sLog->outString("Startup");
    sLog->outString("Using config dir: %s", SERVER_CONFIG);
    if (!ConfigMgr::Load(SERVER_CONFIG))
        return 0;

    sConfiguration->Load();
    sLog->Initialize();
    sNetworkMgr->Open();

    sClientSessionMgr->Open();
    uint16 port = ConfigMgr::GetIntDefault("ListenPort", 39235);
    sClientAcceptor->Open(port, "0.0.0.0");

    sServer->RunUpdateLoop();

    sClientSessionMgr->Close();
    sClientAcceptor->Close();
    sNetworkMgr->Close();

    return 0;
};
