/*
 * Copyright (C) zhoulv2000@163.com
 */

#ifndef _ST_CONNECTION_H_INCLUDED_
#define _ST_CONNECTION_H_INCLUDED_

#include "st_util.h"
#include "st_item.h"
#include "st_buffer.h"

ST_NAMESPACE_BEGIN

// class StTask
// {
// public:
//     virtual int32_t DoEncode() = 0;

//     virtual int32_t DoInput() = 0;

//     virtual int32_t DoProcess() = 0;
    
//     virtual int32_t DoError() = 0;

//     inline void SetDstAddr(struct sockaddr_in* dst)
//     {
//         memcpy(&m_addr_, dst, sizeof(m_addr_));
//     }

//     inline struct sockaddr_in* GetMsgDstAddr()
//     {
//         return &m_addr_;
//     }

// protected:
//     struct sockaddr_in  m_addr_;
// };

class StConnection;

typedef CPP_TAILQ_HEAD<StConnection>    StConnectionQueue;
typedef CPP_TAILQ_ENTRY<StConnection>   StConnectionNext;

class StConnection : public referenceable
{
public:
    StConnection() : 
        m_type_(eUNDEF_CONN), 
        m_task_(NULL),
        m_osfd_(-1), 
        m_buff_(NULL), 
        m_item_(NULL), 
        m_timeout_(ST_MAXINT)
    { }

    virtual ~StConnection()
    { }

    inline eConnType GetConnType()
    {
        return m_type_;
    }

    inline void SetBuffer(StBuffer *buff)
    {
        m_buff_ = buff;
    }

    // 获取message的buffer
    inline StBuffer* GetBuffer()
    {
        return m_buff_;
    }

    inline void SetEventItem(StEventItem *item)
    {
        m_item_ = item;
    }

    inline StEventItem* GetEventItem()
    {
        return m_item_;
    }

    inline int32_t CloseSocket()
    {
        if (m_osfd_ > 0)
        {
            mt_close(m_osfd_);
            m_osfd_ = -1;
        }

        return 0;
    }

    inline int32_t GetOsfd()
    {
        return m_osfd_;
    }

    inline void SetAddr(struct sockaddr_in* dst)
    {
        memcpy(&m_dst_addr_, dst, sizeof(m_dst_addr_));
    }

    inline struct sockaddr_in* GetMsgDstAddr()
    {
        return &m_dst_addr_;
    }

    // 设置超时时间
    inline void SetTimeout(int32_t timeout)
    {
        m_timeout_ = timeout;
    }

    inline int32_t GetTimeout()
    {
        return m_timeout_;
    }

    inline void Reset()
    {
        // 清空buffer
        if (NULL != m_msg_buff_)
        {
            GetInstance<IMsgBufferPool>()->FreeBuffer(m_msg_buff_);
        }
        m_action_ = NULL;
        m_msg_buff_ = NULL;
        m_keep_flag_ = 0;

        memset(&m_dst_addr_, 0, sizeof(m_dst_addr_));
        CloseSocket();
        ResetEventer();
    }

    void ResetEventer();

public:
    virtual int32_t CreateSocket(int32_t fd = -1) = 0;

    virtual int32_t SendData() = 0;

    virtual int32_t RecvData() = 0;

    virtual int32_t OpenConnect()
    {
        return 0;
    }
    
protected:
    eConnType           m_type_;
    StTask*             m_task_;
    StBuffer*        m_buff_;
    StEventItem*        m_item_;

    int32_t             m_osfd_;
    StNetAddress        *m_addr_;
    int32_t             m_timeout_;

public:
    StConnectionNext    m_entry_;
};

// // 请求链接的标识
// class KeepKey : public HashKey
// {
// public:
//     KeepKey()
//     {
//         m_addr_ipv4_  = 0;
//         m_addr_port_   = 0;
//         CPP_TAILQ_INIT(&m_list_);

//         this->SetDataPtr(this);
//     }
//     KeepKey(struct sockaddr_in * dst)
//     {
//         m_addr_ipv4_  = dst->sin_addr.s_addr;
//         m_addr_port_  = dst->sin_port;
//         CPP_TAILQ_INIT(&m_list_);

//         this->SetDataPtr(this);
//     }
//     ~KeepKey()
//     {
//         CPP_TAILQ_INIT(&m_list_);
//     }
//     // 端口和ip组合
//     virtual uint32_t HashValue()
//     {
//         return m_addr_ipv4_ ^ ((m_addr_port_ << 16) | m_addr_port_);
//     }
//     virtual int32_t HashCmp(HashKey* rhs)
//     {
//         KeepKey* data = dynamic_cast<KeepKey*>(rhs);
//         if (!data)
//         {
//             return -1;
//         }

//         if (m_addr_ipv4_ != data->m_addr_ipv4_)
//         {
//             return m_addr_ipv4_ - data->m_addr_ipv4_;
//         }
//         if (m_addr_port_ != data->m_addr_port_)
//         {
//             return m_addr_port_ - data->m_addr_port_;
//         }
        
//         return 0;
//     }
//     void InsertConnection(StConnection *conn)
//     {
//         if (conn->m_keep_flag_ & eKEEP_IN_LIST)
//         {
//             return;
//         }
//         CPP_TAILQ_INSERT_TAIL(&m_list_, conn, m_entry_);
//         conn->m_keep_flag_ |= eKEEP_IN_LIST;
//     }
//     void RemoveConnection(StConnection* conn)
//     {
//         if (!(conn->m_keep_flag_ & eKEEP_IN_LIST))
//         {
//             return ;
//         }
//         CPP_TAILQ_REMOVE(&m_list_, conn, m_entry_);
//         conn->m_keep_flag_ &= ~eKEEP_IN_LIST;
//     }
//     StConnection* GetFirstConnection()
//     {
//         return CPP_TAILQ_FIRST(&m_list_);
//     }

// private:
//     uint32_t        m_addr_ipv4_;
//     uint16_t        m_addr_port_;
//     KeepConnList    m_list_;
// };

// class TcpLongStConnection;
// class HeapTimer;

// // udp的无session连接
// class UdpStConnection : public StConnection
// {
// public:
//     UdpStConnection()
//     {
//         m_osfd_ = -1;
//         m_type_ = eUDP_CONN;
//     }

//     virtual ~UdpStConnection()
//     {
//         StConnection::Reset();
//     }

//     virtual int CreateSocket(int fd = -1);

//     virtual int SendData();

//     virtual int RecvData();
// };

// // tcp短连接
// class TcpShortStConnection : public StConnection
// {
// public:
//     TcpShortStConnection()
//     {
//         m_osfd_ = -1;
//         m_type_ = eTCP_SHORT_CONN;
//     }

//     virtual ~TcpShortStConnection()
//     {
//         StConnection::Reset();
//     }

//     virtual int CreateSocket(int fd = -1);

//     virtual int OpenConnect();

//     virtual int SendData();

//     virtual int RecvData();
// };

// // tcp accept连接
// class TcpAcceptStConnection : public StConnection
// {
// public:
//     TcpAcceptStConnection()
//     {
//         m_osfd_ = -1;
//         m_type_ = eTCP_ACCEPT_CONN;
//     }

//     virtual ~TcpAcceptStConnection()
//     {
//         StConnection::Reset();
//     }

//     virtual int CreateSocket(int fd = -1);

//     // 暂时不需要两个虚函数
//     virtual int SendData()
//     {
//         return 0;
//     }

//     virtual int RecvData()
//     {
//         return 0;
//     }
// };

// class TcpLongStConnection : public TcpShortStConnection, public TimerEntry
// {
// public:
//     TcpLongStConnection() : m_timer_(NULL)
//     {
//         m_osfd_ = -1;
//         m_keep_time_ = 10 * 60 * 1000;
//         m_type_ = eTCP_LONG_CONN;
//     }

//     virtual ~TcpLongStConnection()
//     {
//         StConnection::Reset();
//     }

//     int CreateSocket(int fd = -1);

//     bool IdleAttach();

//     bool IdleDetach();

//     virtual void Notify(eEventType type);

//     // 设置时钟监听控制
//     inline void SetHeapTimer(HeapTimer* timer)
//     {
//         m_timer_ = timer;
//     }

//     inline void SetKeepTimeout(int timeout)
//     {
//         m_keep_time_ = timeout;
//     }

// protected:
//     int             m_keep_time_;
//     HeapTimer*      m_timer_;
// };

// // 管理tcp长连接(单例)
// class TcpLongPool
// {
// public:
//     TcpLongPool() : m_keep_map_(NULL)
//     {
//         m_keep_map_ = new HashList<KeepKey>(10000);
//     }

//     ~TcpLongPool()
//     {
//         if (!m_keep_map_)
//         {
//             return;
//         }

//         HashKey* hash_item = m_keep_map_->HashGetFirst();
//         while (hash_item)
//         {
//             if (hash_item != NULL)
//             {
//                 safe_delete(hash_item);
//             }
//             hash_item = m_keep_map_->HashGetFirst();
//         }
//         safe_delete(m_keep_map_);
//     }

//     TcpLongStConnection* GetConnection(struct sockaddr_in* dst);

//     bool CacheConnection(TcpLongStConnection* conn);

//     bool RemoveConnection(TcpLongStConnection* conn);

//     void FreeConnection(TcpLongStConnection* conn, bool force_free); // 是否强制释放

// private:
//     HashList<KeepKey>*                  m_keep_map_;
//     UtilPtrPool<TcpLongStConnection>  m_tcplong_pool_;
// };

// // 获取不同的连接方式: 
// class ConnectionPool
// {
// public:
//     typedef UtilPtrPool<UdpStConnection>       UdpPool;
//     typedef UtilPtrPool<TcpShortStConnection>  TcpShortPool;

//     StConnection* GetConnection(eConnType type, struct sockaddr_in* dst = NULL);

//     void FreeConnection(StConnection* conn, bool force_free = false); // 是否强制释放

//     // 处理tcp长连接
//     void CloseIdleTcpLong(StConnection* conn);

// private:
//     UdpPool	        m_udp_pool_;
//     TcpShortPool	m_tcpshort_pool_;
//     TcpLongPool     m_tcplong_pool_;
// };

ST_NAMESPACE_END

#endif