/* �ȈՃ`���b�g�v���O���� */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h> /* WinSock�̃w�b�_�t�@�C�� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSock���C�u�����̎w�� */

/* �`���b�g�v���O�����@�T�[�o�[�֐� */
void ChatServer(void)
{
	SOCKET listen_s;
	SOCKET s{};
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	int fromlen;
	u_short uport;

	/* �|�[�g�ԍ��̓��� */
	printf("�g�p����|�[�g�ԍ�--> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* ���X���\�P�b�g���I�[�v�� */
	listen_s = socket(AF_INET, SOCK_DGRAM, 0);
		if (listen_s == INVALID_SOCKET) {
			printf("���X���\�P�b�g�I�[�v���G���[");
			WSACleanup();
			return;
		}
	printf("���X���\�P�b�g���I�[�v�����܂���\n");

	/* �\�P�b�g�ɖ��O��t���� */
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	struct sockaddr_in recv_addr;

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(8000);
	recv_addr.sin_addr.S_un.S_addr = inet_addr("192.168.32.81");

	if (bind(listen_s,(const SOCKADDR *)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("bind�G���[");
		closesocket(listen_s);
		return;
	}
	printf("bind�����ł�");

	///* �N���C�A���g����̐ڑ��҂��̏�Ԃɂ��� */
	//if (listen(listen_s,0) == SOCKET_ERROR) {
	//	printf("listen�G���[\n");
	//	closesocket(listen_s);
	//	return;
	//}
	//printf("listen�����ł�\n");

	///* �ڑ���ҋ@���� */
	//printf("accept�őҋ@���܂�\n");
	//fromlen = (int)sizeof(from);
	//s = accept(listen_s, (SOCKADDR*)&from, &fromlen);
	//	if (s == INVALID_SOCKET) {
	//		printf("accept�G���[\n");
	//		closesocket(listen_s);
	//		return;
	//	}

	//printf("%s���ڑ����Ă��܂���\n", inet_ntoa(from.sin_addr));
	//printf("accepet�֐������ł�\n");

	/* ���X���\�P�b�g�͂����s�v */
	closesocket(listen_s);

	/* ��b�J�n */
	printf("��b�J�n\n");

	while (1) {

		/* ����M�o�b�t�@ */
		int nRcv;
		char szBuf[1024]{};
		printf("��M��҂��Ă��܂�\n");

		fromlen = (int)sizeof(recv_addr);

		/* �f�[�^�̓ǂݏo�� */
		nRcv = recvfrom(s, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&recv_addr, &fromlen);
		/*if (nRcv == SOCKET_ERROR) {
			printf("recv�G���[�ł�\n");
			break;
		}*/
		szBuf[0] = '\0';

		/* �ڑ��ؒf�̃`�F�b�N */
		if (strcmp(szBuf, "c_end") == 0) {
			printf("�N���C�A���g���ڑ���ؒf���܂���\n");
			break;
		}

		/* ��M���b�Z�[�W�̕\�� */
		printf("��M --> %s\n", szBuf);

		/* ���M���b�Z�[�W�̓��� */
		printf("���M --> ");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		/* ���M���b�Z�[�W�𑗂� */
		send(s, szBuf, (int)strlen(szBuf), 0);

		/* �T�[�o�[������ؒf���� */
		if (strcmp(szBuf, "s_end") == 0) {
			break;
		}

	}

	/* �\�P�b�g����� */
	closesocket(s);
}

/* �`���b�g�v���O�����@�N���C�A���g�֐� */
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

	/* �|�[�g�ԍ��̓��� */
	printf("�g�p����|�[�g�ԍ� --> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* �T�[�o���܂��̓T�[�o��IP�A�h���X����� */
	printf("IP�A�h���X -->");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	/* �\�P�b�g���I�[�v�� */
	s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s == INVALID_SOCKET) {
			printf("�\�P�b�g�I�[�v���G���[\n");
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

	/* �T�[�o�[�𖼑O�Ŏ擾���� */
	lpHost = gethostbyname(szServer);
	if (lpHost == NULL) {
		/* �T�[�o�[��IP�A�h���X�Ŏ擾���� */
		addr = inet_addr(szServer);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (lpHost == NULL) {
		printf("Host��������܂���\n");
		closesocket(s);
		return;
	}
	

	/*�@���X���C�h���Q�l��SOCKADDR_IN�^�ϐ��usaddr�v�̐ݒ�@*/
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = lpHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
	/*if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
		closesocket(s);
		return;
	}
	printf("�T�[�o�[�ɐڑ��ł��܂���\n");*/

	while (1) {

		/* ����M�o�b�t�@ */
		int nRcv;
		char szBuf[1024]{};
		

		/* ���M�f�[�^�̓��� */
		printf("���M --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		/* �f�[�^�𑗐M���� */
		sendto(s, szBuf, sizeof(szBuf),0,(struct sockaddr*)&send_addr,sizeof(send_addr));

		/* �ڑ���ؒf���邩�H */
		if (strcmp(szBuf, "c_end") == 0) {
			break;
		}
		fromlen = (int)sizeof(recv_addr);

		/* �T�[�o�[����̃f�[�^����M���� */
		nRcv = recvfrom(s, szBuf, sizeof(szBuf) , 0,(struct sockaddr*)&recv_addr,&fromlen);
		/*if (nrcv == socket_error) {
			printf("��M�G���[\n");
			break;
		}*/
		szBuf[0] = '\0';

		/*if (strcmp(szbuf, "s_end") == 0) {
			printf("�T�[�o�[���ڑ���ؒf���܂���\n");
			break;
		}*/

		 //��M�f�[�^�̕\�� 
		printf("��M --> %s\n", szBuf);

	}

	/* �\�P�b�g����� */
	closesocket(s);
}

/* �`���b�g�v���O���� ���C���֐� */
int main(void)
{
	WSADATA wsaData;
	int mode;

	/* WinSock�̏����� */
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		/* �������G���[ */
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return 1;
	}

	/* �T�[�o�[���H�@�N���C�A���g���H */
	printf("�T�[�o�[�Ȃ�0����� �N���C�A���g�Ȃ�1����� --> ");
	scanf_s("%d", &mode);
	fflush(stdin);

	if (mode == 0) {
		/* �T�[�o�[�Ƃ��ċN�� */
		ChatServer();
	}
	else {
		/* �N���C�A���g�Ƃ��ċN�� */
		ChatClient();
	}

	/* WinSock�̏I������ */
	WSACleanup();

	return 0;
}