// Copyright (C) 2003  Davis E. King (davis@dlib.net), Miguel Grinberg
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_SOCKETS_KERNEl_1_
#define DLIB_SOCKETS_KERNEl_1_

#include "utilities.h" 
#include <string>
#include <cstdlib>
#include <iosfwd>
#include <sstream>

namespace dlib
{
	enum general_return_codes
	{
		TIMEOUT = -1,
		WOULDBLOCK = -2,
		OTHER_ERROR = -3,
		SHUTDOWN = -4,
		PORTINUSE = -5
	};


// ----------------------------------------------------------------------------------------

    // forward declarations
    class socket_factory;
    class listener;
    class SOCKET_container;

// ----------------------------------------------------------------------------------------

    // lookup functions

    int
    get_local_hostname (
        std::string& hostname
    );

// -----------------

    int 
    hostname_to_ip (
        const std::string& hostname,
        std::string& ip,
        int n = 0
    );

// -----------------

    int
    ip_to_hostname (
        const std::string& ip,
        std::string& hostname
    );

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // connection object
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class connection
    {
        /*!
            INITIAL_VALUE
                - sd                      == false
                - sdo                     == false
                - sdr                     == 0

            CONVENTION
                - connection_socket       == the socket handle for this connection.  
                - connection_foreign_port == the port that foreign host is using for 
                  this connection.
                - connection_foreign_ip   == a string containing the IP address of the 
                  foreign host.
                - connection_local_port   == the port that the local host is using for 
                  this connection.
                - connection_local_ip     == a string containing the IP address of the 
                  local interface being used by this connection.

                - sd == if shutdown() has been called then true else false.
                - sdo == if shutdown_outgoing() has been called then true else false.
                - sdr == the return value of shutdown() if it has been called.  if it 
                  hasn't been called then 0.
                        
        !*/

        friend class listener;                // make listener a friend of connection
        // make create_connection a friend of connection
        friend int create_connection ( 
            connection*& new_connection,
            unsigned short foreign_port, 
            const std::string& foreign_ip, 
            unsigned short local_port,
            const std::string& local_ip 
        );

    public:

        ~connection (
        );

        void* user_data;

        long write (
            const char* buf, 
            long num
        );

        long read (
            char* buf, 
            long num
        );

        long read (
            char* buf, 
            long num,
            unsigned long timeout
        );

        unsigned short get_local_port (
        ) const {  return connection_local_port; }

        unsigned short get_foreign_port ( 
        ) const { return connection_foreign_port; }

        const std::string& get_local_ip (
        ) const { return connection_local_ip; }

        const std::string& get_foreign_ip (
        ) const { return connection_foreign_ip; }

        int shutdown_outgoing (
        );

        int shutdown (
        );

        // I would use SOCKET here but I don't want to include the windows
        // header files since they bring in a bunch of unpleasantness.  So
        // I'm doing this instead which should ultimately be the same type
        // as the SOCKET win the windows API.
        typedef UINT_PTR  socket_descriptor_type;

        int disable_nagle(
        );

        socket_descriptor_type get_socket_descriptor (
        ) const;

    private:

        bool readable (
            unsigned long timeout 
        ) const;
        /*! 
            requires 
                - timeout < 2000000  
            ensures 
                - returns true if a read call on this connection will not block. 
                - returns false if a read call on this connection will block or if 
                  there was an error. 
        !*/ 

        bool sd_called (
        )const
        /*!
            ensures
                - returns true if shutdown() has been called else
                  returns false
        !*/
        {
            sd_mutex.lock();
            bool temp = sd;
            sd_mutex.unlock();
            return temp;
        }

        bool sdo_called (
        )const
        /*!
            ensures
                - returns true if shutdown_outgoing() or shutdown() has been called 
                  else returns false
        !*/
        {
            sd_mutex.lock();
            bool temp = false;
            if (sdo || sd)
                temp = true;
            sd_mutex.unlock();
            return temp;
        }


        // data members
        SOCKET_container& connection_socket;
        const unsigned short connection_foreign_port;
        const std::string connection_foreign_ip; 
        const unsigned short connection_local_port;
        const std::string connection_local_ip;

        bool sd;  // called shutdown
        bool sdo; // called shutdown_outgoing
        int sdr; // return value for shutdown 
        mutex sd_mutex; // a lock for the three above vars


        connection(
            SOCKET_container sock,
            unsigned short foreign_port, 
            const std::string& foreign_ip, 
            unsigned short local_port,
            const std::string& local_ip
        ); 
        /*!
            requires
                sock is a socket handle and 
                sock is the handle for the connection between foreign_ip:foreign_port 
                and local_ip:local_port
            ensures
                *this is initialized correctly with the above parameters
        !*/


        // restricted functions
        connection(connection&);        // copy constructor
        connection& operator=(connection&);    // assignment operator
    }; 

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // listener object
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class listener
    {
        /*!
            CONVENTION
                if (inaddr_any == false)
                {
                    listening_ip == a string containing the address the listener is 
                                    listening on
                }
                else
                {
                    the listener is listening on all interfaces
                }
                
                listening_port == the port the listener is listening on
                listening_socket == the listening socket handle for this object
        !*/

        // make the create_listener a friend of listener
        friend int create_listener (
            listener*& new_listener,
            unsigned short port,
            const std::string& ip 
        );

    public:

        ~listener (
        );

        int accept (
            connection*& new_connection,
            unsigned long timeout = 0
        );

        int accept (
            scoped_ptr<connection>& new_connection,
            unsigned long timeout = 0
        );

        unsigned short get_listening_port (
        ) { return listening_port; }

        const std::string& get_listening_ip (
        ) { return listening_ip; }

    private:

        // data members
        SOCKET_container& listening_socket;
        const unsigned short listening_port;
        const std::string listening_ip;
        const bool inaddr_any;

        listener(
            SOCKET_container sock,
            unsigned short port,
            const std::string& ip
        );
        /*!
            requires
                sock is a socket handle                                             and 
                sock is listening on the port and ip(may be "") indicated in the 
                above parameters
            ensures
                *this is initialized correctly with the above parameters
        !*/


        // restricted functions
        listener(listener&);        // copy constructor
        listener& operator=(listener&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------

    int create_listener (
        listener*& new_listener,
        unsigned short port,
        const std::string& ip = ""
    );

    int create_connection ( 
        connection*& new_connection,
        unsigned short foreign_port, 
        const std::string& foreign_ip, 
        unsigned short local_port = 0,
        const std::string& local_ip = ""
    );

    int create_listener (
        scoped_ptr<listener>& new_listener,
        unsigned short port,
        const std::string& ip = ""
    );

    int create_connection ( 
        scoped_ptr<connection>& new_connection,
        unsigned short foreign_port, 
        const std::string& foreign_ip, 
        unsigned short local_port = 0,
        const std::string& local_ip = ""
    );

// ----------------------------------------------------------------------------------------

	struct network_address
	{
		network_address() : port(0){}

		network_address(
			const std::string& full_address
			);

		network_address(
			const char* full_address
			)
		{
			*this = network_address(std::string(full_address));
		}

		network_address(
			const std::string& host_address_,
			const unsigned short port_
			) : host_address(host_address_), port(port_) {}

		std::string host_address;
		unsigned short port;
	};

	// ----------------------------------------------------------------------------------------

	inline bool operator < (
		const network_address& a,
		const network_address& b
		)
	{
		if (a.host_address < b.host_address)
			return true;
		else if (a.host_address > b.host_address)
			return false;
		else if (a.port < b.port)
			return true;
		else
			return false;
	}

	inline bool operator== (
		const network_address& a,
		const network_address& b
		) {
		return a.host_address == b.host_address && a.port == b.port;
	}

	inline bool operator != (
		const network_address& a,
		const network_address& b
		) {
		return !(a == b);
	}

	// ----------------------------------------------------------------------------------------
	std::ostream& operator<< (
		std::ostream& out,
		const network_address& item
		);

	std::istream& operator>> (
		std::istream& in,
		network_address& item
		);

	// ----------------------------------------------------------------------------------------

	connection* connect(
		const std::string& host_or_ip,
		unsigned short port
		);

	// ----------------------------------------------------------------------------------------

	connection* connect(
		const network_address& addr
		);

	// ----------------------------------------------------------------------------------------

	connection* connect(
		const std::string& host_or_ip,
		unsigned short port,
		unsigned long timeout
		);

	// ----------------------------------------------------------------------------------------

	bool is_ip_address(
		std::string ip
		);

	// ----------------------------------------------------------------------------------------
}

#endif // DLIB_SOCKETS_KERNEl_1_

