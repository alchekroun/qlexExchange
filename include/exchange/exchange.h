#pragma once

#include <qlexnet.h>

#include "logger/Logger.h"
#include "messages/Messages.h"

namespace qlexexchange
{
    class Exchange : public qlexnet::ServerInterface<MessageTypes>
    {
    public:
        Exchange(const uint16_t port_) : qlexnet::ServerInterface<MessageTypes>(port_) {};

        void update(const size_t &maxMsg_ = std::numeric_limits<size_t>::max(), bool wait_ = false)
        {
            if (wait_)
                _rxQueue.wait();

            size_t msgCount = 0;
            while (msgCount < maxMsg_ && !_rxQueue.empty())
            {
                auto msg = _rxQueue.pop_front();
                onMessage(msg.remote, msg.msg);
                ++msgCount;
            }
        }

    protected:
        virtual bool onClientConnect(std::shared_ptr<qlexnet::Connection<MessageTypes>> client_)
        {
            qlexnet::Message<MessageTypes> msg;
            msg.header.id = MessageTypes::server_accept;
            client_->send(msg);
            return true;
        }

        virtual void onClientDisconnect(std::shared_ptr<qlexnet::Connection<MessageTypes>> client_)
        {
            std::cout << "Removing client [" << client_->GetID() << "]\n";
        }

        virtual void onMessage(std::shared_ptr<qlexnet::Connection<MessageTypes>> client, qlexnet::Message<MessageTypes> &msg)
        {
            switch (msg.header.id)
            {
            case MessageTypes::ask_info:
            {
                int id_asked;
                msg >> id_asked;
                qlexnet::Message<MessageTypes> msg1;
                msg1.header.id = MessageTypes::ask_info;
                msg1 << "Test Clear.";
                client->send(msg1);
                break;
            }
            case MessageTypes::server_accept:
                break;
            default:
                break;
            }
        }

        void message_client_by_id(uint32_t const id_, const qlexnet::Message<MessageTypes> &msg_)
        {
            auto invalidClientExist = false;

            for (auto &client : _connections)
            {
                if (client->GetID() == id_)
                {
                    if (client && client->isConnected())
                    {
                        client->send(msg_);
                        break;
                    }
                    onClientConnect(client);
                    client.reset();
                    invalidClientExist = true;
                }
            }

            if (invalidClientExist)
                _connections.erase(std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end());
        }
    };
} // qlexExchange