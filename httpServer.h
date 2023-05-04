#include "arpa/inet.h"
#include "bits/stdc++.h"

using namespace std;

namespace http
{
        class TcpServer
        {
        public:
                TcpServer(string ip_address, int portnumber);
                ~TcpServer();
                void startListen();

        private:
                string localIpAddress;
                int localPort;
                int localSocket;
                int localNewSocket;
                long localIncomingMessage;

                struct sockaddr_in localSocketAddress;
                unsigned int localSocketAddressLen;

                string localServerMessage;

                int initServer();
                void endServer();
                void acceptConnection(int &newSocket);
                string buildResponse();
                void sendResponse();
        };
} // namespace http