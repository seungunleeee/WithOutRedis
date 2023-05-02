// c파일cpp로옮기기.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

// ConsoleApplication10.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include<mswsock.h>
#include <ws2tcpip.h>
#include<cstdio>
#include<iostream>
#include"C:\Users\asus\OneDrive\바탕 화면\새 폴더 (6)\how_to_use_redis_lib\cpp\RedisCpp-hiredis\src\CRedisConn.h"
#include<hiredis.h>
#include <vector>
#include <sstream>
#include <list>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

#define BUF_SIZE 1024
#define READ	3
#define	WRITE	5
#define ACCEPT  7
#define START 1
#define SOCKET_POOL_SIZE 10
#define rediskey 0
#define UDPIP 1
#define UDPPORT 2
//Acceptex 관련
LPFN_ACCEPTEX lpfnAcceptEx = NULL;
GUID GuidAcceptEx = WSAID_ACCEPTEX;
WSAOVERLAPPED olOverlap;
DWORD dwBytes;
char lpOutputBuf[1024];
int outBufLen = 1024;
char buffer[1024];
std::list<std::string> TempAdminDB;   //관제
std::list<std::string> TempRiderDB;   //라이더


typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;

} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

//소켓에서 데이터를 받은 후에 여기에있는 변수들에 어떻게 값을넣어주는지
typedef struct    // buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	SOCKET hClntSock;
	SOCKADDR_IN* clntAdr;
	//RedisCpp::CRedisConn con;
	int rwMode;    // READ or WRITE
} PER_IO_DATA, * LPPER_IO_DATA;


#pragma comment(lib , "ws2_32.lib")
BOOL on = TRUE;
DWORD WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void ErrorHandling(const char* message);
std::vector<std::string> split(std::string str, char Delimiter);
int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, i, flags = 0, result = 0;
	int nextSocketIndex = 0;
	//RedisCpp::CRedisConn con;
	SOCKADDR_IN service;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	SOCKADDR_IN clntAdr[SOCKET_POOL_SIZE];



	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);
	//남아있는 애들한태 IO넘길거니까 남아있는애들 포문으로 돌려주면서 걔네한태 뭐해야할지(EchoThreadMain) 넘겨주고 대기하라해.  
	// 요청오면연락할태니까 핸드폰줄게(hComPort) 이거스레드를 몇개를 줄지.
	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		CreateThread(NULL, 0, EchoThreadMain, hComPort, 0, NULL);



	SOCKET socketPool[SOCKET_POOL_SIZE];



	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//if (setsockopt(hServSock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&on, sizeof(on)))return -1;
	/*if (!con.connect("127.0.0.1", 6379))
	{
		std::cout << "connect error " << con.getErrorStr() << std::endl;
		return -1;
	}
	else
	{
		std::cout << "connect success !!!" << std::endl;

	}*/

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(8080);


	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));


	handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
	handleInfo->hClntSock = hServSock;
	handleInfo->clntAdr = servAdr;





	printf(" completion port 결과 : %d", CreateIoCompletionPort((HANDLE)hServSock, hComPort, (ULONG_PTR)handleInfo, 0));



	listen(hServSock, SOMAXCONN);
	printf("일단 리슨하고있어요 8080에서\n");





	int iResult = 0;
	//아래에 있는 accpetex 함수 호출을 위해 필요함
	iResult = WSAIoctl(hServSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx, sizeof(GuidAcceptEx),
		&lpfnAcceptEx, sizeof(lpfnAcceptEx),
		&dwBytes, NULL, NULL);
	if (result == SOCKET_ERROR) {
		printf("WSAIoctl failed with error: %d\n", WSAGetLastError());
		closesocket(hServSock);
		WSACleanup();
		return 1;
	}
	int bytesReceived;

	// for 문 시작



	//  socket 풀 생성,
	for (int i = 0; i < 10; i++) {
		socketPool[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);/* WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);*/
		/*	iResult = setsockopt(hServSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				(char*)&socketPool[i], sizeof(socketPool[i]));
			printf("setsocket opt 설정값 : %d", iResult);*/
			/*int resopt =  setsockopt(socketPool[i], SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)hServSock, sizeof(hServSock));*/
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = socketPool[i];

		printf("서버 소켓이 가져갈 소켓 : %d\n", handleInfo->hClntSock);
		printf(" completion port 결과 : %d", CreateIoCompletionPort((HANDLE)socketPool[i], hComPort, (ULONG_PTR)handleInfo, 0));
		printf("CompletionPort에 등록\n");






	};




	listen(hServSock, SOMAXCONN);
	printf("server 소켓 : %d\n", hServSock);

	for (int i = 0; i < 10; i++) {


		char buf[3];
		memset(buf, 0, sizeof(buf));

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->hClntSock = socketPool[i];

		//acceptex
		ioInfo->rwMode = 10;
		lpfnAcceptEx(hServSock, socketPool[i], &(ioInfo->wsaBuf), 0,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&dwBytes, &(ioInfo->overlapped));





	}
	// for 문 끝






	printf("서버 8080port에서 listen 중");


	while (1)
	{


	}
	return 0;
}

DWORD WINAPI EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;  //LPOVERLAPPED로 나중에바꿔야함 
	DWORD flags = 0;
	DWORD dwBytes;
	/*RedisCpp::CRedisConn con;*/
	//클라이언트 IP주소 얻기위한 변수 
	SOCKADDR_IN* tempUserInfo = 0;

	int port;

	sockaddr* localSockaddr;
	int localSockaddrLength;
	sockaddr* remoteSockaddr;
	int remoteSockaddrLength;

	//get요청받기위한 변수들
	char buf[2048];
	char method[100];
	char ct[100];
	char filename[100];
	struct sockaddr_in* addr_in = 0;
	std::string result;
	std::vector<std::string> ParsedResult;



	uint64_t ret;
	std::list <std::string>::iterator iter;


	while (1)
	{
		// 가끔 다시실행하면 콘솔창에 암것도안뜸; 끄고 다시실행하면되는데 왜그러냐 ㄹㅇ


		GetQueuedCompletionStatus(hComPort, &bytesTrans,
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		printf("쓰레드  내부 GetQueuedCompletionStatus 후 \n");
		sock = handleInfo->hClntSock;
		printf("IO 발생소켓  : %d\n", sock);




		/*if (!con.connect("127.0.0.1", 6379))
		{
			std::cout << "connect error " << con.getErrorStr() << std::endl;
			return -1;
		}
		else
		{
			std::cout << "connect success !!!" << std::endl;

		}*/


		// 요청 처리하는 분기문
		if (ioInfo->rwMode == READ)
		{

			printf("rwMode == Read 분기 시작");
			puts("message received!");

			if (bytesTrans == 0)    // EOF 전송 시
			{
				// 여기서 DisconnectEx() 함수를 이용하여 socket 재활용 처리해줘야함.
				printf("EOF 전송받음!");
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}



			std::cout << " 처음에 IO로 들어온값 : " << ioInfo->buffer << std::endl;
			//라이더 요청 처리
			if (strstr(ioInfo->buffer, "Rider") != NULL)
			{
				std::string RedisValue;
				printf("Rider임\n");
				/*	printf("Rider 위치정보 : %s\n", ioInfo->buffer);*/
				std::string temp = ioInfo->buffer;
				std::cout << "Rider 위치정보 : " << temp << std::endl;
				temp.erase(0, temp.find(',') + 1);
				if (strstr(temp.c_str(), "Enrollment") != NULL) {
					temp.erase(0, temp.find(',') + 1);
					std::cout << "라이더의 Enrollment 요청 왔음 분기 성공" << std::endl;
					std::cout << "parsing 후 값 : " << temp << std::endl;
					ParsedResult = split(temp, '-');

					for (int i = 0; i < 3; i++) {
						std::cout << "라이더의 요청 파싱결과 : " << ParsedResult[i] << std::endl;
					}
					std::vector<std::string> ParsedLocation = split(ParsedResult[rediskey], '_');
					for (int i = 0; i < ParsedLocation.size(); i++) {
						std::cout << "라이더의 위치정보 파싱결과 : " << ParsedLocation[i] << std::endl;
					}



					// 라이더
					char ipstr[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &(ioInfo->clntAdr->sin_addr), ipstr, sizeof(ipstr));
					std::string tcpAddress = ipstr;
					std::string TcpValue;
					TcpValue.append(tcpAddress);
					TcpValue.append("|");
					TcpValue.append(std::to_string(ntohs(ioInfo->clntAdr->sin_port)));
					std::cout << "string 으로 바꾼 TCP 정보" << TcpValue << std::endl;
					std::string keyhead = "Rider";
					RedisValue.append(ParsedResult[UDPIP]);
					RedisValue.append("|");
					RedisValue.append(ParsedResult[UDPPORT]);
					RedisValue.append("_");
					RedisValue.append(TcpValue);

					//  TempRiderDB.push_back(RedisValue); 라이더 정보 저장하는것임
					TempRiderDB.push_back(RedisValue);
					std::cout << "Rider정보 최종적으로 Redis에 저장할 값 : " << RedisValue << std::endl;
				
					/*for (int i = 0; i < ParsedLocation.size(); i++) {
						keyhead.append(ParsedLocation[i]);

						std::cout << "레디스에 푸시 성공여부 : " << con.rpush(keyhead, RedisValue, ret) << std::endl;
						std::cout << "레디스에 HEAD 값  : " << keyhead << std::endl;
						if (i != ParsedLocation.size() - 1) {
							keyhead.append("_");
						}
					}*/
					/*std::cout << "레디스에 HEAD 최종 값  : " << keyhead << std::endl;
					RedisCpp::ValueList resultList;*/
					/*con.lrange("AdministratorDaegu_Susung_Bumadong", 0, 0, resultList);*/

					iter = TempAdminDB.begin();
					std::string ParsedUdpResult;
					int start = 0;
					for (iter; iter != TempAdminDB.end(); iter++)
					{
						std::string temp;
						std::vector<std::string> UDPTCPINFO;
						std::cout << *iter << std::endl; // 원본 리스트: 11 22 33 44 55 
						temp = *iter;
						UDPTCPINFO = split(temp, '_');
						if (start == 0) {
							ParsedUdpResult.append(UDPTCPINFO[0]);
							start += 1;//나중에 #define UDPINFO 어드레스|포트 이런식으로 바꾸자
						}
						else {
							ParsedUdpResult.append("-");
							ParsedUdpResult.append(UDPTCPINFO[0]);
						}
					}

					std::cout << " ParsedUdpResult  결과 : " << ParsedUdpResult << std::endl;

					//EOF 추가
					ParsedUdpResult.append(" \0");

					//   send(sock, "{ \"IP\" : \"15.165.22.113\" , \"PORT\" : 3000 } \0", strlen("{ \"IP\" : \"15.165.22.113\" , \"PORT\" : 3000 } \0"), 0);
					send(handleInfo->hClntSock, ParsedUdpResult.c_str(), strlen(ParsedUdpResult.c_str()), 0);

					ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					memset(&(ioInfo->buffer), 0, sizeof(ioInfo->buffer));
					ioInfo->wsaBuf.len = BUF_SIZE;
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = READ;
					ioInfo->buffer[BUF_SIZE - 1] = '\0';
					ioInfo->rwMode = WRITE;
					ioInfo->hClntSock = sock;
					ioInfo->clntAdr = tempUserInfo;






					/*	sprintf(ioInfo->buffer, ParsedUdpResult.c_str());*/
					WSASend(sock, &(ioInfo->wsaBuf),
						1, NULL, 0, &(ioInfo->overlapped), NULL);


					/*std::string sendString = *iter;*/



					continue;
				}



			}


			//관리자 요청 처리
			else if (strstr(ioInfo->buffer, "Administrator") != NULL) {
				std::string RedisValue;
				std::string temp = ioInfo->buffer;
				tempUserInfo = ioInfo->clntAdr;
				temp.erase(0, temp.find(',') + 1);

				printf("Administrator 임\n");
				printf("Administrator 위치정보 : %s\n", temp.c_str());


				//요청분기
				if (strstr(temp.c_str(), "Enrollment") != NULL) {
					std::cout << "관리자의 Enrollment 요청 왔음 분기 성공" << std::endl;
					temp.erase(0, temp.find(',') + 1);
					std::cout << temp << std::endl;
					ParsedResult = split(temp, '-');

					for (int i = 0; i < 3; i++) {
						std::cout << "관리자 요청 파싱결과 : " << ParsedResult[i] << std::endl;
					}

				}
				std::string keyhead = "Administrator";

				char ipstr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(ioInfo->clntAdr->sin_addr), ipstr, sizeof(ipstr));
				std::string tcpAddress = ipstr;
				std::string TcpValue;
				TcpValue.append(tcpAddress);
				TcpValue.append("|");
				TcpValue.append(std::to_string(ntohs(ioInfo->clntAdr->sin_port)));



				RedisValue.append(ParsedResult[UDPIP]);
				RedisValue.append("|");
				RedisValue.append(ParsedResult[UDPPORT]);
				RedisValue.append("_");
				RedisValue.append(TcpValue);




				printf("Administartor UDP정보/TCP정보  저장할값 : %s\n", RedisValue.c_str());
				/*printf("관제 키 : %s\n", keyhead.c_str());*/

				TempAdminDB.push_back(RedisValue);
			/*	con.lpush(keyhead.append(ParsedResult[rediskey]), RedisValue, ret);*/
				keyhead = "Rider";
				/*RedisCpp::ValueList resultList;

				con.lrange(keyhead.append(ParsedResult[rediskey]), 0, 3, resultList);
				printf("keyHEAD : %s\n", keyhead.c_str());*/
				iter = TempRiderDB.begin();

				std::string result;
				//내가 추가
				std::string ParsedUdpResult;
				int start = 0;
				for (iter; iter != TempRiderDB.end(); iter++)
				{
					std::string temp;
					std::vector<std::string> UDPTCPINFO;
					std::cout << *iter << std::endl; 
					temp = *iter;
					UDPTCPINFO = split(temp, '_');
					if (start == 0) {
						ParsedUdpResult.append(UDPTCPINFO[0]);
						start += 1;//나중에 #define UDPINFO 어드레스|포트 이런식으로 바꾸자 
					}
					else {
						ParsedUdpResult.append("-");
						ParsedUdpResult.append(UDPTCPINFO[0]);
					}
				}

				std::cout << " ParsedUdpResult  결과 : " << ParsedUdpResult << std::endl;







				//con.lrange("RiderDaegu", 0, 0, resultList);
				//iter = resultList.begin();
				//for (iter; iter != resultList.end(); iter++)
				//{
				//	std::cout << *iter << std::endl; // 원본 리스트: 11 22 33 44 55 
				//}




				ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				memset(&(ioInfo->buffer), 0, sizeof(ioInfo->buffer));
				ioInfo->wsaBuf.len = BUF_SIZE;
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->rwMode = READ;
				ioInfo->buffer[BUF_SIZE - 1] = '\0';
				ioInfo->rwMode = WRITE;
				ioInfo->hClntSock = sock;
				ioInfo->clntAdr = tempUserInfo;





				ParsedUdpResult.append(" \0");
				sprintf(ioInfo->buffer, ParsedUdpResult.c_str());
				WSASend(sock, &(ioInfo->wsaBuf),
					1, NULL, 0, &(ioInfo->overlapped), NULL);


				ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				memset(&(ioInfo->buffer), 0, sizeof(ioInfo->buffer));
				ioInfo->wsaBuf.len = BUF_SIZE;
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->rwMode = READ;
				ioInfo->buffer[BUF_SIZE - 1] = '\0';
				ioInfo->rwMode = WRITE;
				ioInfo->hClntSock = sock;
				ioInfo->clntAdr = tempUserInfo;


				WSARecv(sock, &(ioInfo->wsaBuf),
					1, NULL, &flags, &(ioInfo->overlapped), NULL);


				continue;

			}


			char ipstr[INET_ADDRSTRLEN];
			std::string HgetResult;
			tempUserInfo = ioInfo->clntAdr;
			inet_ntop(AF_INET, &(ioInfo->clntAdr->sin_addr), ipstr, sizeof(ipstr));

			/*	con.hget("Rider", ipstr, HgetResult);
				printf("레디스에서 가져온 값 : 내 TCP프로토콜 IP 번호 : %s  내 포트 번호 : %s\n", ipstr, HgetResult.c_str());
				printf("reveived message (http요청시 ioInfo->buffer 뭐가오는지확인용  : %s ", ioInfo->wsaBuf.buf);*/



			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			memset(&(ioInfo->buffer), 0, sizeof(ioInfo->buffer));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			ioInfo->buffer[BUF_SIZE - 1] = '\0';
			ioInfo->rwMode = WRITE;
			ioInfo->hClntSock = sock;
			ioInfo->clntAdr = tempUserInfo;
			printf("문자열길이 : %ld", bytesTrans);
			//이거 wsaSend쓰도록바꿔야함

			sprintf(ioInfo->buffer, "{ \"IP\" : \"15.165.22.113\" , \"PORT\" : 3000 } ");
			WSASend(sock, &(ioInfo->wsaBuf),
				1, NULL, 0, &(ioInfo->overlapped), NULL);


			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			/*memset(&(ioInfo->wsaBuf), 0, sizeof(ioInfo->wsaBuf));*/
			memset(&(ioInfo->buffer), 0, sizeof(ioInfo->buffer));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			ioInfo->buffer[BUF_SIZE - 1] = '\0';
			ioInfo->hClntSock = sock;
			ioInfo->clntAdr = tempUserInfo;
			WSARecv(sock, &(ioInfo->wsaBuf),
				1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		// 응답 처리하는 분기문
		else if (ioInfo->rwMode == WRITE)
		{
			puts("message sent!");

			memset(&(ioInfo->wsaBuf), 0, sizeof(ioInfo->wsaBuf));
			/*closesocket(sock);*/
			free(ioInfo);
		}
		// AcceptEx()요청시 처리하는 분기문. (AcceptEx분기 후 
		else {
			printf("첫요청 소캣 :%d\n", handleInfo->hClntSock);
			printf("앞으로 요청받을 소켓: %d\n", ioInfo->hClntSock);





			/*iResult = WSAIoctl(hServSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&GuidAcceptEx, sizeof(GuidAcceptEx),
				&lpfnAcceptEx, sizeof(lpfnAcceptEx),
				&dwBytes, NULL, NULL);
			if (result == SOCKET_ERROR) {
				printf("WSAIoctl failed with error: %d\n", WSAGetLastError());
				closesocket(hServSock);
				WSACleanup();
				return 1;
			}*/

			/*	handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
				handleInfo->hClntSock = ioInfo->hClntSock;*/

			std::cout << "setsockopt 결과" << setsockopt(ioInfo->hClntSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&handleInfo->hClntSock, sizeof(SOCKET)) << std::endl;
			sock = ioInfo->hClntSock;



			LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs = NULL;
			GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
			WSAOVERLAPPED olOverlap;
			DWORD dwBytes;
			char lpOutputBuf[1024];
			int outBufLen = 1024;
			char buffer[1024];

			int iResult = WSAIoctl(ioInfo->hClntSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&GuidGetAcceptExSockaddrs, sizeof(GuidGetAcceptExSockaddrs),
				&lpfnGetAcceptExSockaddrs, sizeof(lpfnGetAcceptExSockaddrs),
				&dwBytes, NULL, NULL);

			if (iResult == SOCKET_ERROR) {
				printf("WSAIoctl failed with error: %d\n", WSAGetLastError());
				closesocket(ioInfo->hClntSock);
				WSACleanup();
				return 1;
			}

			lpfnGetAcceptExSockaddrs(
				&(ioInfo->wsaBuf),
				0,
				sizeof(SOCKADDR_IN) + 16,
				sizeof(SOCKADDR_IN) + 16,
				&localSockaddr,
				&localSockaddrLength,
				&remoteSockaddr,
				&remoteSockaddrLength
			);

			// 클라이언트 주소추출
			if (remoteSockaddr->sa_family == AF_INET) {
				//IPv4 address
				/*addr_in = (struct sockaddr_in*)remoteSockaddr;*/
				addr_in = (SOCKADDR_IN*)remoteSockaddr;
				char ipstr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(addr_in->sin_addr), ipstr, sizeof(ipstr));
				printf("IPv4 Remote address: %s:%d\n", ipstr, ntohs(addr_in->sin_port));

				uint32_t ret = 0;
				/*ioInfo->con.connect("127.0.0.1", 6379);*/
			/*	con.hset("Rider", ipstr, std::to_string(ntohs(addr_in->sin_port)), ret);
				printf("con set 끝났음 일단");*/
			}
			else if (remoteSockaddr->sa_family == AF_INET6) {
				//IPv6 address
				struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)remoteSockaddr;
				char ipstr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ipstr, sizeof(ipstr));
				printf(" IPv6 Remote address: [%s]:%d\n", ipstr, ntohs(addr_in6->sin6_port));
			}
			else {
				//Unknown address family
				printf("Unknown address family\n");
			}
			//클라이언트 주소추출 끝

			/*std::cout << getpeername(handleInfo->hClntSock, (struct sockaddr*)&clintAdr, &clintAdrSize) << std::endl;*/
			/*std::cout << "에러이유 " <<   WSAGetLastError() << std::endl;
			std::cout << "getpeername 결과 : " << clintAdrSize << std::endl;*/





			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			ioInfo->hClntSock = sock;
			ioInfo->clntAdr = addr_in;
			printf("STart 끝 WSArecv 다시 요청 서버 소켓의 accpetex 끝!\n");

			//  클라이언트 IP주소 , PORT 추출 끝



			//앞으로 통신할 소켓(미리 할당한 소켓풀의 소켓중 하나) 등록
			handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
			handleInfo->hClntSock = sock;




			;

			CreateIoCompletionPort((HANDLE)(ioInfo->hClntSock), hComPort, (ULONG_PTR)handleInfo, 0);
			WSARecv(ioInfo->hClntSock, &(ioInfo->wsaBuf),
				1, NULL, &flags, &(ioInfo->overlapped), NULL);

		}
	}
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



std::vector<std::string> split(std::string str, char Delimiter) {
	std::istringstream iss(str);             // istringstream에 str을 담는다.
	std::string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

	std::vector<std::string> result;

	// istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
	while (getline(iss, buffer, Delimiter)) {
		result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
	}

	return result;
};



// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
