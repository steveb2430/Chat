#include <iostream>
#include <list>
#include <SFML/Network.hpp>

const unsigned short PORT = 15000;
const std::string IPADDRESS("127.0.0.1");//change to suit your needs

void server()
{
	sf::Packet packet;
	sf::Uint8 header;
	sf::Uint64 numbers;
	std::string msg;
	std::string name;

	 // Create a socket to listen to new connections
	sf::TcpListener listener;
	listener.listen(PORT);
	// Create a list to store the future clients
	std::list<sf::TcpSocket*> clients;
	// Create a selector
	sf::SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);
	// Endless loop that waits for new connections
	while (true)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					clients.push_back(client);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*client);
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive it
						if (client.receive(packet) == sf::Socket::Done)
						{
							packet >> header;
							switch (header)
							{
							case 1: //Name(String)
								packet >> name;
								break;

							case 2: //Message(String)
								packet >> msg;
								std::cout << name << " said: " << msg << std::endl;
								break;

							case 3: //Player Position(Uint64)
								packet >> numbers;
								std::cout << "Numbers: " << numbers << std::endl;
								break;
							}
						}
					}
				}
			}
		}
	}
}

int main()
{
	server();
	return 0;
}
