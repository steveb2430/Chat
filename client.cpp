#include <iostream>
#include <list>
#include <SFML/Network.hpp>

const unsigned short PORT = 15000;
const std::string IPADDRESS("127.0.0.1");//change to suit your needs

void client()
{
	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Uint8 header;
	std::string msg;
	std::string name;

	sf::Uint64 pos = 0;
	std::cout << "Name?\n";
	std::getline(std::cin, name);

	if (socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		while (true)
		{
			header = 1;
			packet << header;
			packet << name;
			socket.send(packet);
			packet.clear();

			header = 2;
			packet << header;
			std::getline(std::cin, msg);
			packet << msg;
			socket.send(packet);
			packet.clear();

			header = 3;
			packet << header;
			std::cout << "Sending current position\n";
			pos++; //Pretend this is getPOS();
			packet << pos;
			socket.send(packet);
			packet.clear();
		}
	}
}

int main()
{
	client();
	return 0;
}
