#include "stdafx.h"
#include "NetworkManager.h"



typedef unsigned char uchar;

//출처 http://www.cplusplus.com/forum/general/7142/#msg33079
/*
* Copyright (c) 2009, Helios (helios.vmg@gmail.com)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY HELIOS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL HELIOS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
wchar_t* UTF8_to_WChar(const char* str) {
	long b = 0,
		c = 0;
	if ((uchar)str[0] == BOM8A && (uchar)str[1] == BOM8B && (uchar)str[2] == BOM8C)
		str += 3;
	for (const char* a = str; *a; a++)
		if (((uchar)*a) < 128 || (*a & 192) == 192)
			c++;
	wchar_t* res = new wchar_t[c + 1];
	res[c] = 0;
	for (uchar* a = (uchar*)str; *a; a++) {
		if (!(*a & 128))
			//Byte represents an ASCII character. Direct copy will do.
			res[b] = *a;
		else if ((*a & 192) == 128)
			//Byte is the middle of an encoded character. Ignore.
			continue;
		else if ((*a & 224) == 192)
			//Byte represents the start of an encoded character in the range
			//U+0080 to U+07FF
			res[b] = ((*a & 31) << 6) | a[1] & 63;
		else if ((*a & 240) == 224)
			//Byte represents the start of an encoded character in the range
			//U+07FF to U+FFFF
			res[b] = ((*a & 15) << 12) | ((a[1] & 63) << 6) | a[2] & 63;
		else if ((*a & 248) == 240) {
			//Byte represents the start of an encoded character beyond the
			//U+FFFF limit of 16-bit integers
			res[b] = '?';
		}
		b++;
	}
	return res;
}
long getUTF8size(const wchar_t* str) {
	if (!str)
		return 0;
	long res = 0;
	for (; *str; str++) {
		if (*str < 0x80)
			res++;
		else if (*str < 0x800)
			res += 2;
		else
			res += 3;
	}
	return res;
}

char* WChar_to_UTF8(const wchar_t* str) {
	long fSize = getUTF8size(str);
	char* res = new char[fSize + 2]; //C6386 \0 위치 문제로 버퍼 오버런이 일어남 +1 에서 +2로 수정함
	res[fSize] = 0;
	if (!str)
		return res;
	long b = 0;
	for (; *str; str++, b++) {
		if (*str < 0x80)
			res[b] = (char)*str;
		else if (*str < 0x800) {
			res[b++] = (*str >> 6) | 192;
			res[b] = *str & 63 | 128;
		}
		else {
			res[b++] = (*str >> 12) | 224;
			res[b++] = ((*str & 4095) >> 6) | 128;
			res[b] = *str & 63 | 128;
		}
	}
	return res;
}

DWORD recvTread(LPVOID lParam)
{
	while (1)
	{
		_wsetlocale(LC_ALL, L"korean");
		char messageBuffer[MAX_BUFFER];
		int receiveBytes = recv(Network.listenSocket, messageBuffer, MAX_BUFFER, 0);
		if (receiveBytes > 0)
		{
			char* Responecontext;
			
			vector<wchar_t*> message;

			int command = atoi(strtok_s(messageBuffer, "@", &Responecontext));
			
			/*while (true)
			{
				value = strtok_s(NULL, ";", &Responecontext);
				if (value == nullptr)
				{
					break;
				}
				message.push_back(UTF8_to_WChar(value));
			}*/
			switch (command)
			{
			case 1: //케릭터 선택창의 답장 : 케릭터 리스트
				while (true) //여러개의 케릭터를 반환 받았을때
				{
					char* CharacterData;
					//char* detail;
					CharacterData = strtok_s(NULL, "@", &Responecontext);
					
					if (CharacterData == nullptr)break;
					wprintf(L"%s\n", UTF8_to_WChar(CharacterData));
					GM.SetCharacterData(CharacterData); 
					//TODO: NetworkManager
					//게임마스터 저장하기
					// char* 로 저장한다. 다만 한글은 어쩌지?
					//저장 후 MasterScene에 OK 싸인 날리고 씬 변경하기
				}
				MS.sceneUpdateLock = false;
				
				break;
			}
			
		}
	}
}

NetworkManager::NetworkManager()
{
	listenSocket = NULL;
}

NetworkManager::~NetworkManager()
{
}

int NetworkManager::Connect()
{
	// Winsock Start - winsock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}

	// 1. 소켓생성
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr.S_un.S_addr);
	// 2. 연결요청
	int errcode = connect(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (errcode == SOCKET_ERROR)
	{
		//Error - Fail to connect
		printf("WSACleanup faild with error %d\n", WSAGetLastError());
		// 4. 소켓종료
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		//Login Server Connected
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvTread, NULL, NULL, NULL);
	}	
}

void NetworkManager::Disconnect()
{
	// 4. 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();
}

void NetworkManager::Send(string msg)
{
	_wsetlocale(LC_ALL, L"korean");
	const char* msgbuf = msg.c_str();
	size_t  cn;
	wchar_t wc[MAX_BUFFER] = L"";
	mbstowcs_s(&cn, wc, MAX_BUFFER, msgbuf, MAX_BUFFER);
	Send(wc);
}

void NetworkManager::Send(wchar_t* msg)
{
	int bufferLen;
	char* messge = WChar_to_UTF8(msg);
	for (bufferLen = 0; 1; bufferLen++)
	{
		if (messge[bufferLen] == '\0')
		{
			break;
		}
	}
	int sendBytes = send(listenSocket, messge, bufferLen, 0);
	if (sendBytes > 0)
	{
		wprintf(L"TRACE - send message : %s (%d bytes)\n* Enter Message\n->", msg, bufferLen);
	}
	else
	{
		printf("데이터 전송 실패, 에러 코드 = %u \n", WSAGetLastError());
	}
}
