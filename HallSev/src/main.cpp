#include"INet.h"
#include"MyDB.h"
//#include"Factory.h"
int main(int argc,char *argv[])

{
	CINet net;
	net.GetIp();
	net.InitSock();
/*	CMyDB db;
	//printf("%d\n",db.UpdateScore("123","1",80));
	//printf("%d\n",db.GetScore("123","1"));
	
	vector<Game> game;
	db.GetGameList(game);
	printf("---------game:%s, %d,%s,%s\n",game[0].GameNum,game[0].playnum,game[0].GameName,game[0].GameInfo);
	printf("---------game:%s,%d,%s,%s\n",game[1].GameNum,game[1].playnum,game[1].GameName,game[1].GameInfo);
	printf("---------game:%s,%d,%s,%s\n",game[2].GameNum,game[2].playnum,game[2].GameName,game[2].GameInfo);
	if((db.UpDataGame("1","zhizhi","three"))==0){
	printf("updata fail\n");
	}
	game.clear();
*/
return 1;
}


