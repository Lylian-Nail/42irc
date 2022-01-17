#include "Socket.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

Socket::Socket():
    m_filedesc(-1), m_isBlocking(true),
    m_isReusingAddress(false), m_addressInfo()
{

}

Socket::Socket(AddressInfo const &addressInfo):
    m_isBlocking(true), m_isReusingAddress(false), m_addressInfo(addressInfo)
{
    m_filedesc = socket(
            m_addressInfo.m_ipFamily, m_addressInfo.m_sockType,
            m_addressInfo.m_protocol
    );

    if (m_filedesc < 0)
        throw (SysException(m_filedesc));
}

Socket::Socket(int filedesc, AddressInfo const &addressInfo,
               bool isBlocking, bool isReusingAddress):
    m_filedesc(filedesc), m_isBlocking(isBlocking),
    m_isReusingAddress(isReusingAddress), m_addressInfo(addressInfo)
{

}

Socket::Socket(const Socket &copy):
    m_filedesc(copy.getFiledesc()),
    m_isBlocking(copy.isBlocking()),
    m_isReusingAddress(copy.isReusingAddress()),
    m_addressInfo(copy.getAddressInfos())
{

}

int Socket::getFiledesc() const
{
    return m_filedesc;
}

bool Socket::isBlocking() const
{
    return m_isBlocking;
}

bool Socket::isReusingAddress() const
{
    return m_isReusingAddress;
}

AddressInfo Socket::getAddressInfos() const
{
    return m_addressInfo;
}

void Socket::setBlocking(bool isBlocking)
{
    if (isBlocking && !m_isBlocking)
    {
        // Can not do this because of subject
        /*
        int opts = fcntl(m_filedesc, F_GETFL);
        if (opts < 0)
            throw (SysException(opts));
        opts = opts & ~(O_NONBLOCK);
        opts = fcntl(m_filedesc, F_SETFL, opts);
        if (opts < 0)
            throw (SysException(opts));
        m_isBlocking = true;
        */
        ;
    }
    else if (!isBlocking && m_isBlocking)
    {
        int result = fcntl(m_filedesc, F_SETFL, O_NONBLOCK);
        if (result < 0)
            throw (SysException(result));
        m_isBlocking = false;
    }
}

void Socket::setReusingAddress(bool isReusingAddress)
{
    int value = static_cast<int>(isReusingAddress);

    if (isReusingAddress != m_isReusingAddress)
    {
        int result = setsockopt(
                m_filedesc, SOL_SOCKET, SO_REUSEADDR,
                &value, sizeof(value));
        if (result)
            throw (SysException(result));
        m_isReusingAddress = isReusingAddress;
    }
}

void Socket::close()
{
    int result = ::close(m_filedesc);
    if (result)
        throw (SysException(result));
    m_filedesc = -1;
}

Socket::~Socket()
{

}

Socket &Socket::operator=(const Socket &rhs)
{
    if (this != &rhs)
    {
        m_filedesc = rhs.getFiledesc();
        m_isBlocking = rhs.isBlocking();
        m_isReusingAddress = rhs.isReusingAddress();
        m_addressInfo = rhs.getAddressInfos();
    }
    return *this;
}

void Socket::setSockOpt(int level, int optName, void *value, size_t size)
{
    int result = setsockopt(m_filedesc, level, optName, value, size);
    if (result)
        throw (SysException(result));
}

void Socket::bind(const AddressInfo *newAddress)
{
    if (newAddress)
        m_addressInfo = *newAddress;

    int result = ::bind(
            m_filedesc, m_addressInfo.m_cStyle.ai_addr,
            m_addressInfo.m_cStyle.ai_addrlen
            );
    if (result)
        throw (SysException(result));
}

void Socket::connect(const AddressInfo *remoteAddress)
{
    if (remoteAddress)
        m_addressInfo = *remoteAddress;

    int result = ::connect(
            m_filedesc, m_addressInfo.m_cStyle.ai_addr,
            m_addressInfo.m_cStyle.ai_addrlen
            );
    if (result)
        throw (SysException(result));
}

void Socket::listen(int maxQueue)
{
    int result = ::listen(m_filedesc, maxQueue);
    if (result)
        throw (SysException(result));
}

Socket Socket::accept() const
{
    struct sockaddr_storage storageAddress;
    socklen_t size = sizeof(storageAddress);

    int fd = ::accept(
                m_filedesc,
                reinterpret_cast<struct sockaddr *>(&storageAddress),
                &size
            );
    if (fd < 0)
        throw (SysException(fd));

    AddressInfo addressInfo(reinterpret_cast<struct sockaddr *>
            (&storageAddress), size, SOCK_STREAM, IPPROTO_TCP);
    return Socket(fd, addressInfo);
}

ssize_t Socket::send(void const *buffer, size_t size, int flags)
{
    return ::send(m_filedesc, buffer, size, flags);
}

ssize_t Socket::recv(void *buffer, size_t size, int flags)
{
    return ::recv(m_filedesc, buffer, size, flags);
}

std::ostream &operator<<(std::ostream &os, const Socket &socket)
{
    std::string isBlocking = socket.isBlocking() ?
            "is blocking" : "is non-blocking";
    std::string isReusingAddress = socket.isReusingAddress() ?
            "is reusing address" : "is not reusing address";

    os << "Socket of: " << socket.getAddressInfos() << " has file descriptor: "
    << socket.getFiledesc() << ", " << isBlocking << " and "
    << isReusingAddress;

    return os;
}

Socket::SysException::SysException(int errcode):
        m_errcode(errcode)
{

}

const char *Socket::SysException::what() const throw()
{
    return std::strerror(m_errcode);
}
