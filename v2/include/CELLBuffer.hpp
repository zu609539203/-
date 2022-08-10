#ifndef _CELL_BUFFER_HPP_
#define _CELL_BUFFER_HPP_

#include"CELL.hpp"

class CELLBuffer
{
public:
	CELLBuffer(int nSize = 1024 * 8)
	{
		_nSize = nSize;
		_pBuff = new char[_nSize];
	}

	~CELLBuffer()
	{
		if (_pBuff)
		{
			delete[] _pBuff;
			_pBuff = nullptr;
		}
	}

	// �򻺳������������
	bool push(const char* pData, int nLen)
	{
		/*
		if (_nLastPos + nLen > _nSize)
		{	// д��������ݲ�һ�������ڴ��У�Ҳ���Դ������ݿ���Ǵ��̵ȴ洢����
			// ��Ҫд������ݴ��ڿ��ÿռ�
			int extra = (_nLastPos + nLen) - _nSize;
			if (extra < 8192)
				extra = 8192;
			// ��չ Buff �ռ�
			char* buff = new char[_nSize + extra];
			memcpy(buff, _pBuff, _nLastPos);
			delete[] buff;
			_pBuff = buff;
			_nSize += extra;
		}
		*/

		// �첽���ͣ����ͻ�����δ��ʱ�������򻺳����������
		if (_nLastPos + nLen <= _nSize)
		{
			// ��Ҫ���͵����� ���������ͻ�������β��
			memcpy(_pBuff + _nLastPos, pData, nLen);
			// ���ͻ�������β��ƫ��
			_nLastPos += nLen;
			// ���ͻ�������
			if (_nLastPos == _nSize)
			{
				_BuffFullCount++;
			}

			return true;
		}
		// ���ͻ�������
		else
		{
			_BuffFullCount++;
		}
		return false;
	}

	// ���ͻ������ڵ�����
	int write2socket(SOCKET socketfd)
	{
		int ret = 0;
		// ��������������
		if (_nLastPos > 0)
		{
			ret = send(socketfd, _pBuff, _nLastPos, 0);
			// ��ʼ��ƫ����
			_nLastPos = 0;
			// ���÷��ͻ�������������
			_BuffFullCount = 0;
		}
		return ret;
	}
	// ���� 
///---------------------------------------------------------///
	// ����
	int read4socket(SOCKET socketfd)
	{
		// ������δ��
		if (_nSize - _nLastPos)
		{
			// ���ܿͻ�������
			char* szRecv = _pBuff + _nLastPos;
			int len = recv(socketfd, szRecv, _nSize - _nLastPos, 0);
			// �ͻ����˳�
			if (len <= 0) 
			{
				return len;
			}
			// ����������β������
			_nLastPos += len;
			return len;
		}
		return 0;
	}

	// �������Ƿ�������
	bool hasMsg()
	{
		// �жϵڶ������������ݳ����Ƿ������ϢͷNetMsg_DataHeader����
		if (_nLastPos >= sizeof(NetMsg_DataHeader)) {
			// ֪����Ϣ�壨header + body������
			NetMsg_DataHeader* header = (NetMsg_DataHeader*)_pBuff;
			// �жϵ�ǰ��Ϣ�����Ƿ����������Ϣ����
			return _nLastPos >= header->dataLength;
		}
		return false;
	}

	void pop(int nLen)
	{
		// �����жϣ�����ֵ��С�ڵ�ǰ������βָ���ָ��
		if (_nLastPos - nLen > 0)
		{
			memcpy(_pBuff, _pBuff + nLen, _nLastPos - nLen);
		}
		// 
		_nLastPos = _nLastPos - nLen;
		// �������пռ��ˣ����Լ������������
		if (_BuffFullCount > 0)
		{
			--_BuffFullCount;
		}
	}

	void* data()
	{
		return _pBuff;
	}

private:
	// ���ͻ�����
	char* _pBuff = nullptr;
	// ���ͻ�����β��ָ��
	int _nLastPos = 0;
	// �������ܵĿռ��С���ֽڳ���
	int _nSize = 0;
	// ������д������
	int _BuffFullCount = 0;
};

#endif // !_CELL_BUFFER_HPP_
