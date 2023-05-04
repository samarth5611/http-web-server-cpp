#include "httpServer.h"

using namespace http;

int main(){
        TcpServer server = TcpServer("0.0.0.0" , 8080);
        server.startListen();
        return 0;

}