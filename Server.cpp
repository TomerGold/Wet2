#include "Server.h"

Server::Server(int new_server_id, int new_data_center_id, int new_traffic) : serverId(new_server_id),
                                                                             dataCenterId(new_data_center_id),
                                                                             traffic(new_traffic) {}

int Server::GetServerId() const {
    return serverId;
}

void Server::SetServerId(int new_id) {
    serverId = new_id;
}

int Server::GetDataCenterId() const {
    return dataCenterId;
}

void Server::SetDataCenterId(int new_id) {
    dataCenterId = new_id;
}

int Server::GetTraffic() const {
    return traffic;
}

void Server::SetTraffic(int new_traffic) {
    traffic = new_traffic;
}

bool Server::operator>(const Server &server) {
    if (traffic > server.traffic) {
        return true;
    } else if (traffic == server.traffic) {
        if (serverId > server.serverId) {
            return true;
        }
    }
    return false;
}

bool Server::operator==(const Server &server) {
    return serverId == server.serverId;
}













