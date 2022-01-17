#ifndef SOCKET_HPP
#define SOCKET_HPP

# include "AddressInfo.hpp"

class Socket
{
public:
    Socket();
    explicit Socket(AddressInfo const &addressInfo);
    explicit Socket(
            int filedesc, AddressInfo const &addressInfo = AddressInfo(),
            bool isBlocking = true, bool isReusingAddress = false
    );
    Socket(Socket const &copy);
    virtual ~Socket();

    int getFiledesc() const;
    bool isBlocking() const;
    bool isReusingAddress() const;
    AddressInfo getAddressInfos() const;

    void setBlocking(bool isBlocking);
    void setReusingAddress(bool isReusingAddress);
    void close();
    void setSockOpt(int level, int optName, void *value, size_t size);

    ssize_t send(const char *string, size_t len, int flags = 0);
    ssize_t recv(void *buffer,size_t len, int flags = 0);
    Socket &operator=(Socket const &rhs);

private:
    int                 m_filedesc;
    bool                m_isBlocking;
    bool                m_isReusingAddress;
    AddressInfo         m_addressInfo;
};

std::ostream &operator<<(std::ostream &os, Socket const &socket);

#endif //SOCKET_HPP
