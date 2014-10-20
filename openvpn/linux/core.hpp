//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2013-2014 OpenVPN Technologies, Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

// Linux methods for enumerating the number of cores on machine,
// and binding a thread to a particular core.

#ifndef OPENVPN_LINUX_CORE_H
#define OPENVPN_LINUX_CORE_H

#include <unistd.h>
#include <pthread.h>

namespace openvpn {

  inline int n_cores()
  {
    return sysconf(_SC_NPROCESSORS_ONLN);
  }

  inline int bind_to_core(const int core_id)
  {
    const int num_cores = n_cores();
    if (core_id >= num_cores)
      return EINVAL;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
  }
}

#endif