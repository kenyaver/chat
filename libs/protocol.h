#pragma once

class Protocol{
    public:
    // recv/send commands
    void processRecvCommand();
    void processSendCommand();
    void disconnectClient();
};