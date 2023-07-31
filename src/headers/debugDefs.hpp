/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/28 18:55:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGDEFS_HPP
# define DEBUGDEFS_HPP


# define DP_1   (1U << 1)  /* purple, received bytes print                    */
# define DP_2   (1U << 2)  /* end of HTTP message                             */
# define DP_3   (1U << 3)  /* blue, wrote bytes                               */
# define DP_4   (1U << 4)  /* server: location found                          */
# define DP_5   (1U << 5)  /* main: config file as read                       */
# define DP_6   (1U << 6)  /* utils: host port conversion                     */
# define DP_7   (1U << 7)  /* utils: getfile print                            */
# define DP_8   (1U << 8)  /* main: fully interpreted config file             */
# define DP_9   (1U << 9)  /* launchserver: info: socket open, client closed  */
                           /* connection extablished, recevied connection     */
# define DP_10  (1U << 10) /* BufferManager: request                          */
# define DP_11  (1U << 11) /* Location                                        */
# define DP_12  (1U << 12) /* HTTP* constructors, destructors, etc            */
# define DP_13  (1U << 13) /* methods:                                        */
# define DP_14  (1U << 14) /* bufferManager: message info                     */
# define DP_15  (1U << 15) /* CGI:                                            */


/* debug print options mask */

# ifdef DEBUG_PRINT
#  define DP_MASK (DP_1 | DP_2 | DP_3 | DP_4 | DP_5 | DP_6 | DP_7 | DP_8 | DP_9\
				DP_10 | DP_11 | DP_12 | DP_13)
# else
#  define DP_MASK (DP_5 | DP_9 | DP_14 | DP_15 | DP_3 | DP_10)
# endif


#endif /* DEBUGDEFS_HPP */
