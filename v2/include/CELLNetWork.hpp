#ifndef _CELL_NET_WORK_HPP_
#define _CELL_NET_WORK_HPP_

#include"CELL.hpp"

class CELLNetWork
{
private:
	CELLNetWork()
	{
#ifdef _WIN32
		// ����Win Socket 2.x����
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif

#ifndef _WIN32
		//if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		//	return (1);
		// �����쳣�źţ�Ĭ������»ᵼ�½�����ֹ
		signal(SIGPIPE, SIG_IGN);
#endif // !_WIN32
	}

	~CELLNetWork()
	{
#ifdef _WIN32
		// ���Windows socket����
		WSACleanup();
#endif // _WIN32
	}

public:
	// ����ģʽ�����۵��ö��ٴΣ�Ҳֻ�ᴴ����һ��������˲����γ�ʼ�����绷��
	static void Init()
	{
		static CELLNetWork obj;
	}

private:

};

#endif // !_CELL_NET_WORK_HPP_
