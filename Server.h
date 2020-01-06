#ifndef WET1_SERVER_H
#define WET1_SERVER_H

#include <iostream>
#include "LinkedList.h"


class Server {
    int serverId;
    int dataCenterId;
    int traffic;

public:
    Server() = default;

    Server(int new_server_id, int new_data_center_id, int new_traffic = 0);

    ~Server() = default;

    Server(Server &server) = default;

    Server &operator=(Server &server) = default;

    int GetServerId() const;

    void SetServerId(int new_id);

    int GetDataCenterId() const;

    void SetDataCenterId(int new_id);

    int GetTraffic() const;

    void SetTraffic(int new_traffic);

    bool operator>(const Server &server);

    bool operator==(const Server &server);

};


#endif
