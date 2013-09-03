#include "../Paizhaoyi_Log/Log.h"
#include "../Paizhaoyi_Network/Network.h"

int main()
{
	Log::Start();
	Network::Start("127.0.0.1", 8888);

	return 0;
}