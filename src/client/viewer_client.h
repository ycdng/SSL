#pragma once

#include "execution_manager.h"
#include <libwebsockets.h>
#include <iostream>
#include <json/json.h>
#include <vector>
#include <thread_queue.h>
#include <atomic>
/**
 * @brief The per_session_data__minimal struct
 *
 * One of these is created for each client connecting to us
 */
struct per_session_data_minimal
{
  struct per_session_data_minimal* pss_list;
  struct lws* wsi;
  int last;
};

namespace rhoban_ssl
{
namespace viewer
{
/**
 * @brief The ViewerDataGlobal class contains all the data exchanged between the monitor and the ia
 * in the form of json packets.
 */
class ViewerDataGlobal
{
private:
  /**
   * @brief Constructor.
   */
  ViewerDataGlobal();

  /**
   * @brief The singleton of the class.
   */
  static ViewerDataGlobal instance_;

public:
  /**
   * @brief Get the unique instance of the class.
   */
  static ViewerDataGlobal& get();

  /**
   * @brief Store all packets that will be send to clients.
   */
  ThreadQueue<Json::Value> packets_to_send;

  /**
   * @brief All packet received from the viewer.
   */
  ThreadQueue<Json::Value> received_packets;

  /**
   * @brief client_connected
   */
  std::atomic<bool> client_connected;

  /**
   * @brief Parse and add store a packet send by the viewer.
   * @param packet_received in char*.
   */
  void parseAndStorePacketFromClient(char* packet_received);
};

/**
 * @brief The ViewerServerLauncher task permit to launch and close the thread of
 * the viewer websocket server.
 *
 * A thread is launch when the task in created and when the execution manager removes it
 * the thread is automatically close.
 *
 */
class ViewerServerLauncher : public Task
{
private:
  /**
   * @brief The class is use to communicate with the viewer.
   *
   * Currently only one client is supported.
   * Plus, due to the use of libwebsockets all the communication process
   * must be handle in a single thread.
   * To ensure that the ViewerServerLauncher is the only class that contains and can
   * instanciate this class.
   */
  class ViewerServer
  {
  public:
    /**
     * @brief The protocols structure.
     *
     * Use for register all protocols.
     * The tab is set to three because we have only three protocols registred.
     */
    static struct lws_protocols protocols_[3];

    /**
     * @brief The context structure
     * The struct lws_context represents the server.
     */
    static struct lws_context* context_;

    /**
     * @brief Constructor.
     *
     * Create context, protocols and websocket.
     */
    ViewerServer();

    /**
     * @brief Destructor
     *
     * Destroy the context.
     */
    ~ViewerServer();

    /**
     * @brief run
     */
    static void run();

    /**
     * @brief running_
     */
    static std::atomic<bool> running_;

    /**
     * @brief All clients used.
     */
    static std::vector<struct lws*> clients_;

    /**
     * @brief A dummy callback because the libwebsocket doesn't start if the http is not handled.
     * @param wsi Opaque websocket instance pointer
     * @param reason The reason for the call
     * @param user Pointer to per-session user data allocated by library
     * @param in Pointer used for some callback reasons
     * @param len Length set for some callback reasons
     * @return is_success Boolean to see if the callback has success.
     */
    static int callback_http_dummy(struct lws* wsi, enum lws_callback_reasons, void*, void*, size_t);

    /**
     *
     * @brief The callback for the viewer handler.
     * @param wsi Opaque websocket instance pointer
     * @param reason The reason for the call
     * @param user Pointer to per-session user data allocated by library
     * @param in Pointer used for some callback reasons
     * @param len Length set for some callback reasons
     * @return 0 Boolean to say the callback has success.
     */
    static int callback_viewer(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);
  };

private:
  ViewerServer viewer_client_;
  std::thread* thread_;

  /**
   * @brief This member is used to assure that this task is add only once in the execution manager.
   */
  static uint instance_counter_;

public:
  ViewerServerLauncher();
  ~ViewerServerLauncher();

  // Task interface
public:
  /**
   * @brief runTask
   *
   * Use to launch the thread of the viewer client.
   * Also, the task is alive if the thread running.
   * When the task is destroy, it's stop the thread of the viewer client.
   *
   * @return boolean to activate/desactivate the task
   */
  bool runTask();
};

}  // namespace viewer
}  // namespace rhoban_ssl
