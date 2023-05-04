#include "httpServer.h"

#include "bits/stdc++.h"
#include "unistd.h"
#include "sstream"


using namespace std;

namespace
{
        const int BUFFER_SIZE = 30720;
        void log(const string &message)
        {
                cout << message << endl;
        }

        void exitingWithError(const string &errorMessage)
        {
                log("Error : " + errorMessage);
                exit(1);
        }
}

namespace http
{
        TcpServer::TcpServer(string ip_address, int port) : 
        localIpAddress(ip_address),
        localPort(port),
        localSocket(), 
        localNewSocket(), 
        localIncomingMessage(), 
        localSocketAddress(), 
        localSocketAddressLen(sizeof(localSocketAddress)), 
        localServerMessage(buildResponse()){
                // naming the socket and socket-address
                localSocketAddress.sin_family = AF_INET; // socket-family
                localSocketAddress.sin_port = htons(localPort);  // assining socket with port
                localSocketAddress.sin_addr.s_addr = inet_addr(localIpAddress.c_str());                              

                if(initServer() != 0){
                        ostringstream ss;
                        ss <<"Failed to start server with port number: "<< ntohs(localSocketAddress.sin_port);
                       log((ss.str()));
                }
        }

        TcpServer::~TcpServer(){
                endServer();
        }

        int TcpServer::initServer(){
                localSocket = socket(AF_INET , SOCK_STREAM, 0);
                
                if(localSocket < 0){
                        exitingWithError("Not able to create a socket!");
                        return 1;
                }

                if(bind(localSocket , (sockaddr*)&localSocketAddress , localSocketAddressLen) < 0){
                        exitingWithError("Cannot Connect socket to address");
                        return 1;
                }

                return 0;
        }

        void TcpServer::endServer(){
                close(localSocket);
                close(localNewSocket);
                exit(0);
        }

        void TcpServer :: startListen(){
                if(listen(localSocket, 20) < 0){
                        exitingWithError("Socket List failed");
                }
                ostringstream ss;
                ss << "\n*** Listening on ADDRESS: " << inet_ntoa(localSocketAddress.sin_addr) << " PORT: " << ntohs(localSocketAddress.sin_port) << " ***\n\n";
                log(ss.str());

                int bytesReceived;
/*
--------Creating new connection----------
------ Received Request from client -----
*/
                while(true){
                        log("--------Creating new connection----------\n\n\n");
                        acceptConnection(localNewSocket);

                        char buffer[BUFFER_SIZE] = {0};    
                        bytesReceived  = read(localNewSocket , buffer , BUFFER_SIZE);

                        if(bytesReceived < 0){
                                exitingWithError("Failed to read bytes from client socket connection");
                        }

                        ostringstream ss;
                        ss << "------ Received Request from client -----\n\n\n";
                        log(ss.str());

                        sendResponse();
                        close(localNewSocket);
                }
        }

        void TcpServer :: acceptConnection(int &newSocket){
                newSocket = accept(localSocket, (sockaddr *)&localSocketAddress, &localSocketAddressLen);
                       if (newSocket < 0){
                                std::ostringstream ss;
                                ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(localSocketAddress.sin_addr) << "; PORT: " << ntohs(localSocketAddress.sin_port);
                                exitingWithError(ss.str());
                        }
        }
        string TcpServer::buildResponse(){
                string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
                ostringstream ss;
                ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"<< htmlFile;
                return ss.str();
        }

        void TcpServer::sendResponse(){
                long bytesSent;
                bytesSent = write(localNewSocket, localServerMessage.c_str(), localServerMessage.size());

                if (bytesSent == localServerMessage.size()){
                        log("------ Server Response sent to client ------\n\n");
                }
                else{
                        log("Error sending response to client");
                }
    }
}
