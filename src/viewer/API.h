#pragma once

#include <thread>
#include <vision/ai_vision_client.h>
#include <com/ai_commander.h>
#include <joystick/Joystick.h>
#include <json/json.h>
#include <string>
#include <QObject>
#include <data.h>
#include <ai.h>
#include "client_config.h"

class API : public QObject
{
  Q_OBJECT

public:
  API(std::string teamName, bool simulation, rhoban_ssl::ai::Team team, rhoban_ssl::AICommander* commander,
      const std::string& config_path, rhoban_ssl::Vision::PartOfTheField part_of_the_field_used,
      std::string addr = SSL_VISION_ADDRESS, std::string port = SSL_VISION_PORT,
      std::string sim_port = SSL_SIMULATION_VISION_PORT);
  virtual ~API();

  bool simulation;

signals:

public slots:
  bool isSimulation();
  bool isYellow();

  // Vision communication statistics
  QString visionStatus();

  // Status of the referee
  QString refereeStatus();

  // Get the status of all robots
  QString robotsStatus();

  // Gets the status of the ball
  QString ballStatus();

  // Gets the geometry of the field
  QString fieldStatus();

  // Scan for robots
  void scan();

  // Enable/disable a robot
  void enableRobot(int id, bool enabled);

  // Manual control a robot
  void manualControl(int id, bool manual);

  // Active the robot
  void activeRobot(int id, bool active);

  // Commands a robot
  void robotCommand(int id, double xSpeed = 0, double ySpeed = 0, double thetaSpeed = 0);

  // Enable or disable the charge for a robot
  void robotCharge(int id, bool charge = false);

  // Run a kick
  void kick(int id, int kick, float power = 1.0);

  // Set spin
  void setSpin(int id, bool spin);

  // Emergency stop
  void emergencyStop();

  // Setting the position of the ball or of robot
  void moveBall(double x, double y);
  void moveRobot(bool yellow, int id, double x, double y, double theta);

  // Joystick
  QString availableJoysticks();
  void openJoystick(int robot, QString name);
  void stopJoystick();

  // Tweak the PID of a robot (currently a hack to send hard-coded PID values)
  void tweakPid(int id);

  // Get the annotations that needs to be drawn on the screen
  QString getAnnotations();

  // Getting the strategies
  QString getStrategies();
  void updateAssignments();
  void applyStrategy(int id, QString name);
  void clearAssignments();

  // Getting managers
  QString getManagers();
  void setManager(QString manager);
  void managerStop();
  void managerPlay();

protected:
  std::string teamName;
  rhoban_ssl::AI* ai;
  rhoban_ssl::Data data;
  rhoban_ssl::ai::Team team;
  rhoban_ssl::AIVisionClient visionClient;
  rhoban_ssl::AICommander* commander;

  std::map<int, std::string> assignments;

  std::string ourColor();
  std::string opponentColor();

  std::thread* comThread;
  std::thread* aiThread;

  std::mutex mutex;

  std::thread* joystickThread;
  rhoban_ssl::Joystick* joystick;
  int joystickRobot;

  // void comThreadExec();
  void aiThreadExec();
  void joystickThreadExec();
};
