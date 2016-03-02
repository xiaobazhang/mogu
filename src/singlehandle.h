#ifndef __SINGHANDLE_H__
#define __SINGHANDLE_H__
#include <foundation/ckit_singleton.h>
#include <foundation/ckit_mailbox.h>
#include <foundation/ckit_lf_queue.h>
#include <foundation/ckit_lock.h>

using namespace std;
using namespace ckit;


struct IpLog
{
	string log;
	string ip;
};
/**
 * 单例类，用于同步读取log四个线程和一个处理线程
 */
class SingleLogQueue
{
public:
	SingleLogQueue():iTimeOutMs(-1)//阻塞，直到读到数据
	{
		m_messageNum =0;
	}
	~SingleLogQueue()
	{
	}
	void Send(IpLog* pIplog)
	{
		m_MailBoxR.Send(pIplog);
	}
	void Recv(IpLog** pIplog)
	{ 
		m_MailBoxR.Recv(*pIplog,iTimeOutMs);
	}
	static SingleLogQueue* GetInstance()
	{
		static ckit::SingletonHolder<SingleLogQueue> m_single;
		return m_single.Get();
	}
	void count()
	{
		m_messageNum++;
		if(m_messageNum%100==0)
		{
			std::cout<<"m_messageNum="<<m_messageNum<<std::endl;
		}
	}
	int64 m_messageNum;
	MailBoxR<char*> m_MetricMailBoxR;
	MailBoxR<char*> m_AlarmMailBoxR;
private:
	MailBoxR<IpLog*> m_MailBoxR;
	int iTimeOutMs;
};

#endif
