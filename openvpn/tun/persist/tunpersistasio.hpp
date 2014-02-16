//
//  tunpersistasio.hpp
//  OpenVPN
//
//  Copyright (c) 2014 OpenVPN Technologies, Inc. All rights reserved.
//

#ifndef OPENVPN_TUN_PERSIST_TUNPERSISTASIO_H
#define OPENVPN_TUN_PERSIST_TUNPERSISTASIO_H

#include <openvpn/tun/persist/tunpersist.hpp>

namespace openvpn {

  // This object supports that subset of the Asio stream
  // interface required by TunIO, and is intended to wrap
  // a ScopedAsioStream embedded in a TunPersist object.
  // It is used primarily on Windows to wrap the TAP
  // interface HANDLE in way that plays well with Windows
  // I/O completion ports (once a HANDLE is bound to an
  // I/O completion port it cannot be unbound).
  template <typename TunPersist>
  class TunPersistAsioStream
  {
  public:
    TunPersistAsioStream(const typename TunPersist::Ptr& tun_persist_arg)
      : tun_persist(tun_persist_arg) {}


    void release()
    {
      tun_persist.reset();
    }

    // Delegate STREAM methods (only need to support the
    // subset of methods used by TunIO).
    // Prototypes from boost/asio/windows/basic_stream_handle.hpp

    template <typename MutableBufferSequence, typename ReadHandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
				  void (boost::system::error_code, std::size_t))
    async_read_some(const MutableBufferSequence& buffers,
		    BOOST_ASIO_MOVE_ARG(ReadHandler) handler)
    {
      return tun_persist->obj()->async_read_some(buffers, handler);
    }

    template <typename ConstBufferSequence>
    std::size_t write_some(const ConstBufferSequence& buffers)
    {
      return tun_persist->obj()->write_some(buffers);
    }

    void cancel()
    {
      tun_persist->obj()->cancel();
    }

    void close()
    {
      tun_persist->obj()->close();
    }

  private:
    typename TunPersist::Ptr tun_persist;
  };

}
#endif