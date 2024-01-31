/* 簡易チャットプログラム */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSockライブラリの指定 */

/* チャットプログラム　サーバー関数 */
void ChatServer(void)
{
	SOCKET listen_s;
	SOCKET s{};
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	int fromlen;
	u_short uport;

	/* ポート番号の入力 */
	printf("使用するポート番号--> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* リスンソケットをオープン */
	listen_s = socket(AF_INET, SOCK_DGRAM, 0);
		if (listen_s == INVALID_SOCKET) {
			printf("リスンソケットオープンエラー");
			WSACleanup();
			return;
		}
	printf("リスンソケットをオープンしました\n");

	/* ソケットに名前を付ける */
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	struct sockaddr_in recv_addr;

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(8000);
	recv_addr.sin_addr.S_un.S_addr = inet_addr("192.168.32.81");

	if (bind(listen_s,(const SOCKADDR *)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("bindエラー");
		closesocket(listen_s);
		return;
	}
	printf("bind成功です");

	///* クライアントからの接続待ちの状態にする */
	//if (listen(listen_s,0) == SOCKET_ERROR) {
	//	printf("listenエラー\n");
	//	closesocket(listen_s);
	//	return;
	//}
	//printf("listen成功です\n");

	///* 接続を待機する */
	//printf("acceptで待機します\n");
	//fromlen = (int)sizeof(from);
	//s = accept(listen_s, (SOCKADDR*)&from, &fromlen);
	//	if (s == INVALID_SOCKET) {
	//		printf("acceptエラー\n");
	//		closesocket(listen_s);
	//		return;
	//	}

	//printf("%sが接続してきました\n", inet_ntoa(from.sin_addr));
	//printf("accepet関数成功です\n");

	/* リスンソケットはもう不要 */
	closesocket(listen_s);

	/* 会話開始 */
	printf("会話開始\n");

	while (1) {

		/* 送受信バッファ */
		int nRcv;
		char szBuf[1024]{};
		printf("受信を待っています\n");

		fromlen = (int)sizeof(recv_addr);

		/* データの読み出し */
		nRcv = recvfrom(s, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&recv_addr, &fromlen);
		/*if (nRcv == SOCKET_ERROR) {
			printf("recvエラーです\n");
			break;
		}*/
		szBuf[0] = '\0';

		/* 接続切断のチェック */
		if (strcmp(szBuf, "c_end") == 0) {
			printf("クライアントが接続を切断しました\n");
			break;
		}

		/* 受信メッセージの表示 */
		printf("受信 --> %s\n", szBuf);

		/* 送信メッセージの入力 */
		printf("送信 --> ");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		/* 送信メッセージを送る */
		send(s, szBuf, (int)strlen(szBuf), 0);

		/* サーバー側から切断する */
		if (strcmp(szBuf, "s_end") == 0) {
			break;
		}

	}

	/* ソケットを閉じる */
	closesocket(s);
}

/* チャットプログラム　クライアント関数 */
void ChatClient()
{
	SOCKET s;
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	u_short uport;
	char szServer[1024] = { 0 };
	HOSTENT* lpHost;
	unsigned int addr;
	int fromlen;

	/* ポート番号の入力 */
	printf("使用するポート番号 --> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* サーバ名またはサーバのIPアドレスを入力 */
	printf("IPアドレス -->");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	/* ソケットをオープン */
	s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s == INVALID_SOCKET) {
			printf("ソケットオープンエラー\n");
			return;
		}

	struct sockaddr_in send_addr;

	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(8000);
	send_addr.sin_addr.S_un.S_addr = inet_addr("192.168.32.81");

	struct sockaddr_in recv_addr;

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(8000);
	recv_addr.sin_addr.S_un.S_addr = inet_addr("192.168.32.81");

	/* サーバーを名前で取得する */
	lpHost = gethostbyname(szServer);
	if (lpHost == NULL) {
		/* サーバーをIPアドレスで取得する */
		addr = inet_addr(szServer);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (lpHost == NULL) {
		printf("Hostが見つかりません\n");
		closesocket(s);
		return;
	}
	

	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = lpHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
	/*if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("サーバーと接続できませんでした\n");
		closesocket(s);
		return;
	}
	printf("サーバーに接続できました\n");*/

	while (1) {

		/* 送受信バッファ */
		int nRcv;
		char szBuf[1024]{};
		

		/* 送信データの入力 */
		printf("送信 --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		/* データを送信する */
		sendto(s, szBuf, sizeof(szBuf),0,(struct sockaddr*)&send_addr,sizeof(send_addr));

		/* 接続を切断するか？ */
		if (strcmp(szBuf, "c_end") == 0) {
			break;
		}
		fromlen = (int)sizeof(recv_addr);

		/* サーバーからのデータを受信する */
		nRcv = recvfrom(s, szBuf, sizeof(szBuf) , 0,(struct sockaddr*)&recv_addr,&fromlen);
		/*if (nrcv == socket_error) {
			printf("受信エラー\n");
			break;
		}*/
		szBuf[0] = '\0';

		/*if (strcmp(szbuf, "s_end") == 0) {
			printf("サーバーが接続を切断しました\n");
			break;
		}*/

		 //受信データの表示 
		printf("受信 --> %s\n", szBuf);

	}

	/* ソケットを閉じる */
	closesocket(s);
}

/* チャットプログラム メイン関数 */
int main(void)
{
	WSADATA wsaData;
	int mode;

	/* WinSockの初期化 */
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		/* 初期化エラー */
		printf("WinSockの初期化に失敗しました\n");
		return 1;
	}

	/* サーバーか？　クライアントか？ */
	printf("サーバーなら0を入力 クライアントなら1を入力 --> ");
	scanf_s("%d", &mode);
	fflush(stdin);

	if (mode == 0) {
		/* サーバーとして起動 */
		ChatServer();
	}
	else {
		/* クライアントとして起動 */
		ChatClient();
	}

	/* WinSockの終了処理 */
	WSACleanup();

	return 0;
}