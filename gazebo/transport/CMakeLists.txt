include (${gazebo_cmake_dir}/GazeboUtils.cmake)

set (sources
  CallbackHelper.cc
  Connection.cc
  ConnectionManager.cc
  IOManager.cc
  Node.cc
  Publication.cc
  PublicationTransport.cc
  Publisher.cc
  Subscriber.cc
  SubscriptionTransport.cc
  TopicManager.cc
  Transport.cc
) 

set (headers
  CallbackHelper.hh
  Connection.hh
  ConnectionManager.hh
  IOManager.hh
  Node.hh
  Publication.hh
  Publisher.hh
  PublicationTransport.hh
  SubscribeOptions.hh
  Subscriber.hh
  SubscriptionTransport.hh
  TopicManager.hh
  Transport.hh
  TransportTypes.hh
)

set (transport_headers "" CACHE INTERNAL "transport headers" FORCE)
foreach (hdr ${headers})
  APPEND_TO_CACHED_STRING(transport_headers "Transport Headers"
    "#include \"transport/${hdr}\"\n")
endforeach()
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/transport.hh.in
                ${CMAKE_CURRENT_BINARY_DIR}/transport.hh )


gz_add_library(gazebo_transport ${sources})
target_link_libraries(gazebo_transport
  gazebo_common 
  gazebo_msgs 
  ${Boost_LIBRARIES}
  ${TBB_LIBRARIES} 
)

gz_install_library(gazebo_transport)
gz_install_includes("transport" ${headers} ${CMAKE_CURRENT_BINARY_DIR}/transport.hh)
