#pragma once

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH      100
#define BOARD_HEIGHT     100

#define VIEW_RADIUS        3

#define MAX_USER         100

#define NPC_START       1000
#define NUM_OF_NPC     10000

#define MY_SERVER_PORT  7711

#define MAX_STR_SIZE     100

#define CS_MOVE            3

#define SC_POS             1
#define SC_PUT_PLAYER      2
#define SC_REMOVE_PLAYER   3
#define SC_CHAT            4

#pragma pack (push, 1)
// ���� ���� ���� push�ϰ� n¥�� ���İ����� �ٲ۴�. 
// �� push�� ���� ���� �����ϴ� Ű�����̰� n�� �� ���İ��� �ο��ϴ� Ű������ ���̴�
// 1 ����Ʈ ���� (����Ʈ �е�)

struct cs_packet_move
{
	unsigned char size;
	unsigned char type;
	unsigned char dir;
};

struct cs_packet_chat
{
	unsigned char size;
	unsigned char type;
	WCHAR message[MAX_STR_SIZE];
};


struct sc_packet_pos
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
	unsigned char x;
	unsigned char y;
};

struct sc_packet_put_player
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
	unsigned char x;
	unsigned char y;
};

struct sc_packet_remove_player
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
};

struct sc_packet_chat
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
	WCHAR message[MAX_STR_SIZE];
};

#pragma pack (pop)